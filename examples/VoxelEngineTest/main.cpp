#include <Nazara/Core/Clock.hpp>
#include <Nazara/Graphics.hpp>
#include <Nazara/Renderer.hpp>
#include <Nazara/Utility.hpp>
#include <Nazara/VoxelEngine/VoxelEngine.hpp>
#include <Nazara/VoxelEngine/VoxelTerrain.hpp>
#include <iostream>

// Petite fonction permettant de rendre le déplacement de la caméra moins ridige
NzVector3f DampedString(const NzVector3f& currentPos, const NzVector3f& targetPos, float frametime, float springStrength = 3.f);

int main()
{

	NzInitializer<NzVoxelEngine> voxelEngine;
	if (!voxelEngine)
	{
		// Une erreur s'est produite dans l'initialisation d'un des modules
		std::cout << "Failed to initialize Nazara, see NazaraLog.log for further informations" << std::endl;
		std::getchar(); // On laise le temps de voir l'erreur

		return EXIT_FAILURE;
	}

	NzScene scene;

	NzVoxelTerrain terrain;
	terrain.SetParent(scene);
	terrain.Init();

	NzTexture* texture = new NzTexture;
	if (texture->LoadCubemapFromFile("resources/skybox-space.png"))
	{
		texture->SetPersistent(false);
		NzSkyboxBackground* background = new NzSkyboxBackground(texture);
		scene.SetBackground(background);
	}
	else
	{
		delete texture; // Le chargement a échoué, nous libérons la texture
		std::cout << "Failed to load skybox" << std::endl;
	}

	NzEulerAnglesf camAngles(0.f, -20.f, 0.f);

	NzCamera camera;
	camera.SetPosition(0.f, 0.25f, 2.f); // On place la caméra à l'écart
	camera.SetRotation(camAngles);
	camera.SetZFar(5000.f);
	camera.SetZNear(0.1f);
	scene.SetViewer(camera);

	NzLight nebulaLight(nzLightType_Directional);
	nebulaLight.SetColor(NzColor(255, 182, 90));
	nebulaLight.SetRotation(NzEulerAnglesf(0.f, 102.f, 0.f));
	nebulaLight.SetParent(scene);

	NzVideoMode mode = NzVideoMode::GetDesktopMode();

	mode.width *= 3.f/4.f;
	mode.height *= 3.f/4.f;

	NzString windowTitle = "VoxelEngineDemo";

	nzWindowStyleFlags style = nzWindowStyle_Default;

	NzRenderTargetParameters parameters;
	parameters.antialiasingLevel = 4;

	NzRenderWindow window(mode, windowTitle, style, parameters);
	if (!window.IsValid())
	{
		std::cout << "Failed to create render window" << std::endl;
		std::getchar();

		return EXIT_FAILURE;
	}

	window.SetCursor(nzWindowCursor_None);

	camera.SetTarget(window);

	NzClock secondClock, updateClock;

	unsigned int fps = 0;

	bool smoothMovement = true;
	NzVector3f targetPos = camera.GetPosition();
	bool camMode = true;

	while (window.IsOpen())
	{
		NzEvent event;
		while (window.PollEvent(&event))
		{
			switch (event.type)
			{
				case nzEventType_MouseMoved:
				{
				    if(!camMode)
                        break;

					float sensitivity = 0.3f;

					camAngles.yaw = NzNormalizeAngle(camAngles.yaw - event.mouseMove.deltaX*sensitivity);

					camAngles.pitch = NzClamp(camAngles.pitch - event.mouseMove.deltaY*sensitivity, -89.f, 89.f);

					camera.SetRotation(camAngles);

					NzMouse::SetPosition(window.GetWidth()/2, window.GetHeight()/2, window);
					break;
				}

				case nzEventType_MouseButtonPressed:
					if (event.mouseButton.button == NzMouse::Left)
					{
						// L'utilisateur vient d'appuyer sur le bouton left de la souris
						// Nous allons inverser le mode caméra et montrer/cacher le curseur en conséquence
						if (camMode)
						{
							camMode = false;
							window.SetCursor(nzWindowCursor_Default);
						}
						else
						{
							camMode = true;
							window.SetCursor(nzWindowCursor_None);
						}
					}
                    break;

				case nzEventType_Quit:
					window.Close();
					break;

				case nzEventType_KeyPressed:
					if (event.key.code == NzKeyboard::Key::Escape)
						window.Close();
					else if (event.key.code == NzKeyboard::F1)
					{
						if (smoothMovement)
						{
							targetPos = camera.GetPosition();
							smoothMovement = false;
						}
						else
							smoothMovement = true;
					}
					break;

				default:
					break;
			}
		}

		if (updateClock.GetMilliseconds() >= 1000/60)
		{
			float elapsedTime = updateClock.GetSeconds();

			float cameraSpeed = 3.f * elapsedTime;

			if (NzKeyboard::IsKeyPressed(NzKeyboard::Space))
				cameraSpeed *= 2.f;

			if (NzKeyboard::IsKeyPressed(NzKeyboard::Up) || NzKeyboard::IsKeyPressed(NzKeyboard::Z))
				targetPos += camera.GetForward() * cameraSpeed;

			if (NzKeyboard::IsKeyPressed(NzKeyboard::Down) || NzKeyboard::IsKeyPressed(NzKeyboard::S))
				targetPos += camera.GetBackward() * cameraSpeed;

			if (NzKeyboard::IsKeyPressed(NzKeyboard::Left) || NzKeyboard::IsKeyPressed(NzKeyboard::Q))
				targetPos += camera.GetLeft() * cameraSpeed;

			if (NzKeyboard::IsKeyPressed(NzKeyboard::Right) || NzKeyboard::IsKeyPressed(NzKeyboard::D))
				targetPos += camera.GetRight() * cameraSpeed;

			if (NzKeyboard::IsKeyPressed(NzKeyboard::LShift) || NzKeyboard::IsKeyPressed(NzKeyboard::RShift))
				targetPos += NzVector3f::Up() * cameraSpeed;

			if (NzKeyboard::IsKeyPressed(NzKeyboard::LControl) || NzKeyboard::IsKeyPressed(NzKeyboard::RControl))
				targetPos += NzVector3f::Down() * cameraSpeed;

			camera.SetPosition((smoothMovement) ? DampedString(camera.GetPosition(), targetPos, elapsedTime) : targetPos, nzCoordSys_Global);

			updateClock.Restart();
		}


		scene.Update();
		scene.Cull();
		scene.UpdateVisible();
		scene.Draw();
		window.Display();

		fps++;

		if (secondClock.GetMilliseconds() >= 1000)
		{
			window.SetTitle(windowTitle + " - " + NzString::Number(fps) + " FPS");

			fps = 0;
			secondClock.Restart();
		}
	}

    return EXIT_SUCCESS;
}

NzVector3f DampedString(const NzVector3f& currentPos, const NzVector3f& targetPos, float frametime, float springStrength)
{
	// Je ne suis pas l'auteur de cette fonction
	// Je l'ai reprise du programme "Floaty Camera Example" et adaptée au C++
	// Trouvé ici: http://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/opengl_programming.html#4
	// Tout le mérite revient à l'auteur (Qui me permettra ainsi d'améliorer les démos, voire même le moteur)

	// calculate the displacement between the target and the current position
	NzVector3f displacement = targetPos - currentPos;

	// whats the distance between them?
	float displacementLength = displacement.GetLength();

	// Stops small position fluctuations (integration errors probably - since only using euler)
	if (NzNumberEquals(displacementLength, 0.f))
		return currentPos;

	float invDisplacementLength = 1.f/displacementLength;

	const float dampConstant = 0.000065f; // Something v.small to offset 1/ displacement length

	// the strength of the spring increases the further away the camera is from the target.
	float springMagitude = springStrength*displacementLength + dampConstant*invDisplacementLength;

	// Normalise the displacement and scale by the spring magnitude
	// and the amount of time passed
	float scalar = std::min(invDisplacementLength * springMagitude * frametime, 1.f);
	displacement *= scalar;

	// move the camera a bit towards the target
	return currentPos + displacement;
}
