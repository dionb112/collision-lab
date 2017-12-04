/// <summary>
/// Collision experiments
/// @author Dion Buckley
/// @date Nov 2017
/// 
/// TODO:	
///		Circle to Ray
///		Ray to AABB
///		Ray to Cap
///		Ray to Circle
///		Ray to Poly
/// 
/// I.E:
///		1. Enable NPC to swap between AABB, Capsule, Poly, Ray, Circle. [DONE]
/// 
///		2. Enable player to swap between AABB, Circle and Ray [DONE] 
/// 
/// Known Bugs: Ray as player not at 100% 
///				
/// </summary>
/// <returns></returns>

#include <iostream>
#include <SFML/Graphics.hpp>
#define TINYC2_IMPL
#include <tinyc2.h>

using namespace std;

enum class CurrentMouseShape {
	DEFAULT,
	AABB,
	CIRCLE,
	RAY
} currentMouse;

enum class CurrentNPCShape {
	AABB,
	CIRCLE,
	RAY,
	POLY,
	CAPSULE
} currentNPC;

/// <summary>
/// function to take in current mouse texture and update it based on currentMouseShape
/// </summary>
sf::Texture setMouseTexture(sf::Texture t_mouseTexture);

/// <summary>
/// function to take in current npc texture and update it based on currentNPCShape
/// </summary>
sf::Texture setNPCTexture(sf::Texture t_npcTexture);

int main()
{
	// Collision result
	int result = 0;

	// Default player to pointer
	currentMouse = CurrentMouseShape::DEFAULT;
	currentNPC = CurrentNPCShape::AABB;

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Collision Lab");

	// Setup instruction text
	sf::Text text;
	sf::Font font;
	if (!font.loadFromFile("assets\\BAUHS93.ttf"))
	{
		std::cout << " Error with font loading";
	}
	text.setFont(font);
	text.setPosition(sf::Vector2f{ 25,10 });
	text.setCharacterSize(30);
	text.Italic;
	text.setFillColor(sf::Color::Magenta);
	text.setString(
		"Choose Player Shape using Function Keys \n : F1 for AABB \n , F2 for Circle \n , F3 for Ray. \nChoose NPC Shape using Letter Keys \n : A for AABB \n , C for Capsule \n , O for Circle \n , P for Poly \n , R for Ray."
		);

	// Load first texture to display on shape
	sf::Texture npcTexture;
	if (!npcTexture.loadFromFile("assets\\aabb.png")) 
	{
		cout << "Error loading shape grid" << endl;
	}

	// Default mouse pointer
	sf::Texture mouseTexture;
	if (!mouseTexture.loadFromFile("assets\\pointer_mouse.png"))
		{
			cout << "Error loading mouse pointer" << endl;

		}

	//Setup manifold for collisions
	c2Manifold manifold;
	manifold.count = 0;

	// Setup a mouse Sprite
	sf::Sprite mouseSprite;
	mouseSprite.setTexture(mouseTexture);

	// Setup Player AABB
	c2AABB aabbPlayer;

	// Setup Player Circle
	c2Circle circlePlayer;
	circlePlayer.r = 44;

	// Setup player ray
	c2Ray rayPlayer;
	rayPlayer.d = c2Norm(c2V(1, 0));
	rayPlayer.t = 88;

	// Setup NPC Sprite
	sf::Sprite npcSprite;
	npcSprite.setTexture(npcTexture);

	//Setup NPC AABB
	c2AABB aabbNPC;
	aabbNPC.min = c2V(400, 400);
	aabbNPC.max = c2V(
		aabbNPC.min.x +
		mouseSprite.getGlobalBounds().width,
		aabbNPC.min.y +
		mouseSprite.getGlobalBounds().height
	);

	// Setup NPC Circle
	c2Circle circleNPC;
	circleNPC.p = c2V(400 + circlePlayer.r, 400 + circlePlayer.r);
	circleNPC.r = 44;

	// Setup NPC ray
	c2Ray rayNPC;
	rayNPC.p = c2V(400, 440);
	rayNPC.d = c2Norm(c2V(1, 0));
	rayNPC.t = 88;

	// Setup NPC capsule
	c2Capsule capsuleNPC;
	capsuleNPC.r = 16.5f; // as in asset
	capsuleNPC.a = c2V(400 + capsuleNPC.r , 430 + capsuleNPC.r);
	capsuleNPC.b = c2V(
		capsuleNPC.a.x +
		mouseSprite.getGlobalBounds().width - capsuleNPC.r * 2,
		capsuleNPC.a.y
	);

	// Setup NPC Poly
	c2Poly polyNPC;
	polyNPC.count = 4;
	polyNPC.verts[0] = c2V(400, 400);
	polyNPC.verts[1] = c2V(
		polyNPC.verts[0].x + 
		mouseSprite.getGlobalBounds().width, 
		polyNPC.verts[0].y);
	polyNPC.verts[2] = c2V(
		polyNPC.verts[1].x,
		polyNPC.verts[1].y +
		mouseSprite.getGlobalBounds().width);
	polyNPC.verts[3] = c2V(
		polyNPC.verts[0].x,
		polyNPC.verts[2].y);
	c2MakePoly(&polyNPC);


	// Set defaultt enemy sprite pos
	npcSprite.setPosition(aabbNPC.min.x, aabbNPC.min.y);

	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		mouseSprite.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));


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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
				{
					currentMouse = CurrentMouseShape::AABB;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
				{
					currentMouse = CurrentMouseShape::CIRCLE;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
				{
					currentMouse = CurrentMouseShape::RAY;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
				{
					currentNPC = CurrentNPCShape::CIRCLE;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
				{
					currentNPC = CurrentNPCShape::RAY;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
				{
					currentNPC = CurrentNPCShape::POLY;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				{
					currentNPC = CurrentNPCShape::AABB;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
				{
					currentNPC = CurrentNPCShape::CAPSULE;
				}
				break;
			}
		}

		// Update texture map for mouse sprite
		mouseTexture = setMouseTexture(mouseTexture);
		mouseSprite.setTexture(mouseTexture);

		// Update texture map for NPC sprite
		npcTexture = setNPCTexture(npcTexture);
		npcSprite.setTexture(npcTexture);
		
		// Update shapes and Check for collisions
		if (currentMouse == CurrentMouseShape::AABB)
		{
			// Update player aabb (as moving with mouse)
			aabbPlayer.min = c2V(
				mouseSprite.getPosition().x,
				mouseSprite.getPosition().y
			);
			aabbPlayer.max = c2V(
				aabbPlayer.min.x +
				mouseSprite.getGlobalBounds().width,
				aabbPlayer.min.y +
				mouseSprite.getGlobalBounds().height
			);

			if (currentNPC == CurrentNPCShape::AABB)
			{
				//AABB to AABB (as is default in Phil Starter)
				result = c2AABBtoAABB(aabbPlayer, aabbNPC);
			}
			else if (currentNPC == CurrentNPCShape::RAY)
			{
				// AABB to Ray
				// Raycast
				c2Raycast cast;
				result = c2RaytoAABB(rayNPC, aabbPlayer, &cast);
			}
			else if (currentNPC == CurrentNPCShape::CAPSULE)
			{
				// AABB to Cap
				 result = c2AABBtoCapsule(aabbPlayer, capsuleNPC);
			}
			else if (currentNPC == CurrentNPCShape::POLY)
			{
				// AABB to Poly
				result = c2AABBtoPoly(aabbPlayer, &polyNPC, NULL);
			}	
			else if (currentNPC == CurrentNPCShape::CIRCLE)
			{
				// AABB to Circle
				result = c2CircletoAABB(circleNPC, aabbPlayer );
			}

		}
		// Circle 
		else if (currentMouse == CurrentMouseShape::CIRCLE)
		{
			// Update player circle (as moving with mouse)
			circlePlayer.p = c2V(
				mouseSprite.getPosition().x + circlePlayer.r,
				mouseSprite.getPosition().y + circlePlayer.r
			);
			if (currentNPC == CurrentNPCShape::AABB)
			{
				// Circle to AABB
				result = c2CircletoAABB(circlePlayer, aabbNPC);
			}
			else if (currentNPC == CurrentNPCShape::RAY)
			{
				// Circle to Ray
				// Raycast
				c2Raycast cast;
				result = c2RaytoCircle(rayNPC, circlePlayer, &cast);
			}
			else if (currentNPC == CurrentNPCShape::CAPSULE)
			{
				// Circle to Cap
				result = c2CircletoCapsule(circlePlayer, capsuleNPC);
			}
			else if (currentNPC == CurrentNPCShape::POLY)
			{
				// Circle to Poly
				result = c2CircletoPoly(circlePlayer, &polyNPC, NULL);

			}
			else if (currentNPC == CurrentNPCShape::CIRCLE)
			{
				// Circle to Circle
				result = c2CircletoCircle(circlePlayer, circleNPC);
			}
		}
		// Ray Collisions
		else if (currentMouse == CurrentMouseShape::RAY)
		{
			// Update player ray (as moving with mouse)
			rayPlayer.p = c2V(
				mouseSprite.getPosition().x,
				mouseSprite.getPosition().y + 40
			);
			rayPlayer.d.x = mouseSprite.getPosition().x - rayPlayer.p.x;
			rayPlayer.d.y = mouseSprite.getPosition().y - rayPlayer.p.y;
			rayPlayer.d = c2Norm(rayPlayer.d);
			
			if (currentNPC == CurrentNPCShape::AABB)
			{
				// Ray to AABB
				// Raycast
				c2Raycast cast;
				result = c2RaytoAABB(rayPlayer, aabbNPC, &cast);
			}
			else if (currentNPC == CurrentNPCShape::CAPSULE)
			{
				// Ray to Cap
				// Raycast
				c2Raycast cast;
				result = c2RaytoCapsule(rayPlayer, capsuleNPC, &cast);
			}
			else if (currentNPC == CurrentNPCShape::POLY)
			{
				// Ray to Poly
				// Raycast
				c2Raycast cast;
				result = c2RaytoPoly(rayPlayer, &polyNPC, NULL, &cast);
			}
			else if (currentNPC == CurrentNPCShape::CIRCLE)
			{
				// Ray to Circle
				// Raycast
				c2Raycast cast;
				result = c2RaytoCircle(rayPlayer, circleNPC, &cast);
			}
		}
		
		cout << ((result != 0) ? ("Collision") : "") << endl;
		if (result) {
			mouseSprite.setColor(sf::Color(255, 0, 0, 255));
		}
		else {
			mouseSprite.setColor(sf::Color(125, 255, 0, 255));
		}

		// Clear screen
		window.clear(sf::Color::White);

		// Draw stuff
		window.draw(text);

		window.draw(npcSprite);

		window.draw(mouseSprite);

		// Update the window
		window.display();
	}

	return 1;
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
		if (!t_mouseTexture.loadFromFile("assets\\aabb.png")) {
			cout << "Failed to load AABB texture";
		}
	}
	else if (currentMouse == CurrentMouseShape::CIRCLE)
	{
		if (!t_mouseTexture.loadFromFile("assets\\circle.png")) {
			cout << "Failed to load circle texture";
		}
	}
	else if (currentMouse == CurrentMouseShape::RAY)
	{
		if (!t_mouseTexture.loadFromFile("assets\\ray.png")) {
			cout << "Failed to load ray texture";
		}
	}
	else
	{
		if (!t_mouseTexture.loadFromFile("assets\\pointer_mouse.png"))
		{
			cout << "Error loading mouse pointer texture" << endl;
		}
	}
	return t_mouseTexture;
}

sf::Texture setNPCTexture(sf::Texture t_npcTexture)
{
	// Load appropriate NPC texture to display
	if (currentNPC == CurrentNPCShape::AABB)
	{
		if (!t_npcTexture.loadFromFile("assets\\aabb.png")) {
			cout << "Failed to load AABB texture";
		}
	}
	else if (currentNPC == CurrentNPCShape::CIRCLE)
	{
		if (!t_npcTexture.loadFromFile("assets\\circle.png")) {
			cout << "Failed to load circle texture";
		}
	}
	else if (currentNPC == CurrentNPCShape::RAY)
	{
		if (!t_npcTexture.loadFromFile("assets\\ray.png")) {
			cout << "Failed to load ray texture";
		}
	}
	else if (currentNPC == CurrentNPCShape::CAPSULE)
	{
		if (!t_npcTexture.loadFromFile("assets\\capsule.png")) {
			cout << "Failed to load capsule texture";
		}
	}
	else if (currentNPC == CurrentNPCShape::POLY)
	{
		if (!t_npcTexture.loadFromFile("assets\\poly.png")) {
			cout << "Failed to load poly texture";
		}
	}
	return t_npcTexture;
}
