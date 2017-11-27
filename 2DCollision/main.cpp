/// <summary>
/// Collision experiments
/// @author Dion Buckley
/// @date Nov 2017
/// </summary>
/// <returns></returns>

#include <iostream>
#include <SFML/Graphics.hpp>
#define TINYC2_IMPL
#include <tinyc2.h>
#include <AnimatedSprite.h>
#include <Player.h>
#include <Input.h>
#include <Debug.h>

enum class CurrentMouseShape {
	START,
	AABB,
	CIRCLE,
	RAY
} currentMouse;

using namespace std;

sf::Texture setMouseTexture(sf::Texture);

int main()
{
	// Default mouse ( No shape yet )
	currentMouse = CurrentMouseShape::START;

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	// Load a sprite to display
	sf::Texture sprite_sheet;
	if (!sprite_sheet.loadFromFile("assets\\grid.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Declare a mouse texture to load later
	sf::Texture mouseTexture;
	if (!mouseTexture.loadFromFile("assets\\pointer_mouse.png")) {
		DEBUG_MSG("Failed to default mouse texture");
		return EXIT_FAILURE;
	}
	// Setup a mouse Sprite
	sf::Sprite mouse;
	mouse.setTexture(mouseTexture);

	//Setup mouse AABB
	c2AABB aabb_mouse;
	aabb_mouse.min = c2V(mouse.getPosition().x, mouse.getPosition().y);
	aabb_mouse.max = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().width);

	//Setup mouse Circle
	c2Circle circleMouse;
	circleMouse.p.x = mouse.getPosition().x;
	circleMouse.p.y = mouse.getPosition().y;

	// Setup Players Default Animated Sprite
	AnimatedSprite animated_sprite(sprite_sheet);
	animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Setup Players AABB
	c2AABB aabb_player;
	aabb_player.min = c2V(animated_sprite.getPosition().x, animated_sprite.getPosition().y);
	aabb_player.max = c2V(animated_sprite.getGlobalBounds().width / animated_sprite.getFrames().size(), 
		animated_sprite.getGlobalBounds().height / animated_sprite.getFrames().size());


	// Setup the Player
	Player player(animated_sprite);
	Input input;

	// Collision result
	int result = 0;
	
	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		mouse.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

		if (currentMouse == CurrentMouseShape::AABB)
		{
			// Update mouse AABB
			aabb_mouse.min = c2V(mouse.getPosition().x, mouse.getPosition().y);
			aabb_mouse.max = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().width);
		}
		else if (currentMouse == CurrentMouseShape::CIRCLE)
		{
			circleMouse.p.x = mouse.getPosition().x;
			circleMouse.p.y = mouse.getPosition().y;
		}
		

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				// Close window : exit
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					input.setCurrent(Input::Action::LEFT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					input.setCurrent(Input::Action::RIGHT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					input.setCurrent(Input::Action::UP);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
				{
					currentMouse = CurrentMouseShape::AABB;
					mouseTexture = setMouseTexture(mouseTexture);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
				{
					currentMouse == CurrentMouseShape::CIRCLE;
					mouseTexture = setMouseTexture(mouseTexture);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
				{
					currentMouse == CurrentMouseShape::RAY;
					mouseTexture = setMouseTexture(mouseTexture);
				}
				break;
			default:
				input.setCurrent(Input::Action::IDLE);
				break;
			}
		}
		//set mouse texture 
		mouse.setTexture(mouseTexture);

		// Handle input to Player
		player.handleInput(input);

		// Update the Player
		player.update();

		// Check for collisions
		result = c2AABBtoAABB(aabb_mouse, aabb_player);
		cout << ((result != 0) ? ("Collision") : "") << endl;
		if (result){
			player.getAnimatedSprite().setColor(sf::Color(255,0,0));
		}
		else {
			player.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
		}

		// Clear screen
		window.clear();

		// Draw the Players Current Animated Sprite
		window.draw(player.getAnimatedSprite());
		window.draw(mouse);

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};

/// <summary>
/// Function to set mouse texture to new shape for collision experiments
/// Tried passing mouseTexture as reference but had trouble initialising 
/// it as a reference above in main, so this way seemed legit
/// </summary>
/// <param name="t_mouseTexture">texture to be applied to mouse sprite</param>
/// <returns>same but changed</returns>
sf::Texture setMouseTexture(sf::Texture t_mouseTexture)
{
	// Load appropriate mouse texture to display
	if (currentMouse == CurrentMouseShape::AABB)
	{
		if (!t_mouseTexture.loadFromFile("assets\\AABB_mouse.png")) {
			cout << "Failed to load AABB mouse texture";
		}
	}
	else if (currentMouse == CurrentMouseShape::CIRCLE)
	{
		if (!t_mouseTexture.loadFromFile("assets\\circle_mouse.png")) {
			cout << "Failed to load circle mouse texture";
		}
	}
	return t_mouseTexture;
}
