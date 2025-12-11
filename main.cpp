#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<ctime>
#include<cstdlib>
#include<string>
#include<fstream>

struct Bird {
	sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(34.0f, 24.0f));
	sf::Texture texUp;
	sf::Texture texMid;
	sf::Texture texDown;

	float velocity=0.0f;
	int currentFrame = 0;
	float frameTimer = 0.0f;
};

int main() {
	unsigned int width = 800;
	unsigned int height = 600;
	//from Frame Animations clockwise Simulation
	sf::Clock dtClock;

	sf::RenderWindow window(sf::VideoMode({width,height}),"Flappy Bird");
	window.setFramerateLimit(60);
	sf::Texture bgTexture;
	
	if (!bgTexture.loadFromFile("background.png")) {
		std::cout << "Error loading the background"<<std::endl;
		return -1;
	}



	sf::Sprite bgSprite1(bgTexture);
	sf::Sprite bgSprite2(bgTexture);
	float scale = 600.0f / bgTexture.getSize().y;
	bgSprite1.setScale({scale,scale});
	bgSprite2.setScale({ scale,scale });


	bgSprite1.setPosition({ 0.0f,0.0f });

	float ScaleWidth = bgTexture.getSize().x * scale;

	bgSprite2.setPosition({ ScaleWidth,0.0f });

	Bird bird; // we define a bird under our components
	//this is where we define the size of the bird,initially it is a rectangle sized

	if ((!bird.texUp.loadFromFile("bluebird-upflap.png"))||
		(!bird.texMid.loadFromFile("bluebird-midflap.png"))||
		(!bird.texDown.loadFromFile("bluebird-downflap.png"))){
		return -1;
	}

	bird.shape.setTexture(&bird.texMid);
	bird.shape.setPosition({ 100.0f,300.0f });
	bird.velocity = 0.0f;
	float gravity = 0.5f; //How Strong is Gravity
	//center of mass for the bird
	bird.shape.setOrigin({ 17.0f,12.0f });

	//Gap and Pipe speed
	float pipeSpeed = -5.0f;
	float Gap = 150.0f;

	//Declaring 
	sf::Texture pipeTexture;
	if (!pipeTexture.loadFromFile("pipe.png")) {
		return -1;
	}

	//Top Pipe properties
	sf::RectangleShape TopPipe({ 60.0f,600.0f });
	TopPipe.setPosition({ 800.0f,200.0f });
	TopPipe.setTexture(&pipeTexture);
	TopPipe.setScale({ 1.0f,-1.0f });
	//Bottom Pipe properties
	sf::RectangleShape  BottomPipe({ 60.0f,300.0f });
	BottomPipe.setPosition({ 800.0f,400.0f });
	BottomPipe.setTexture(&pipeTexture);

	sf::Font font;

	if (!font.openFromFile("GILSANUB.TTF")) {
		std::cout << "Error loading Font" << std::endl;
		return -1;
	}

	sf::Texture numberTextures[10];
	for (int i = 0; i <= 9; i++) {
		std::string fileName = std::to_string(i) + ".png";

		if (!numberTextures[i].loadFromFile(fileName)) {
			std::cout << "Error loading" << fileName << std::endl;
		}
	}

	//Background Music
	sf::Music music;
	
	if (!music.openFromFile("dbz.mp3")) {
		std::cout << "Error getting Music" << std::endl;
		return -1;
	}

	music.setLooping(true);
	music.setVolume(30.0f);
	music.play();

	//Effects
	sf::SoundBuffer wing, hit;
	if (!wing.loadFromFile("audio_wing.wav") || !hit.loadFromFile("audio_hit.wav")) {
		std::cout << "Error loading audio" <<std::endl;
	}
	sf::Sound wingsnd(wing);
	sf::Sound hitsnd(hit);

	int score = 0;
	bool scorecounted = false;
	bool isGameOver = false;

	sf::Texture gameOverTexture;
	if (!gameOverTexture.loadFromFile("gameover.png")) {
		std::cout << "Error loading endscreen" << std::endl;
		return -1;
	}

	sf::Sprite gameOverSprite(gameOverTexture);

	sf::Vector2u goSize = gameOverTexture.getSize();
	gameOverSprite.setOrigin({ goSize.x / 2.0f,goSize.y / 2.0f });

	gameOverSprite.setPosition({ width / 2.0f,height / 2.0f});

	std::srand(std::time(0)); //generates a random number every frame

	//read from the File
	int highScore = 0;
	std::ifstream readFile("highscore.txt");
	if (readFile.is_open()) {
		readFile >> highScore;
		readFile.close();
	}
	//Text Display

	sf::Text highScoreText(font);
	highScoreText.setCharacterSize(40);
	highScoreText.setFillColor(sf::Color::Yellow);
	highScoreText.setString("Best: " + std::to_string(highScore));
	highScoreText.setPosition({ 300.0f,350.0f });

	while (window.isOpen()) {

		//check all the window's events that were triggered since the last iteration of the loop
		while (const std::optional event = window.pollEvent())
		{
			//"close requested" event: we close the loop
			if (event->is<sf::Event::Closed>())
				window.close();

			//Jump Check
			//First here it is checked that if a button was pressed

			if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
			{
				//If the key pressed was a space
				if (keyEvent->code == sf::Keyboard::Key::Space && !isGameOver)
				{
					//Kick it 10f higher
					bird.velocity = -10.0f;
					wingsnd.play();
				}

				if (keyEvent->code == sf::Keyboard::Key::Enter && isGameOver) {
					music.play();
					score = 0;
					isGameOver = false;

					bird.shape.setPosition({ 100.0f,300.0f });
					bird.velocity = 0.0f;
					bird.shape.setRotation(sf::degrees(0));

					TopPipe.setPosition({ 800.0f,200.0f });
					BottomPipe.setPosition({ 800.0f,400.0f });
				}
				

			}
		}

		//Updating Physics

		if (!isGameOver) {

			sf::Time dt = dtClock.restart();
			bird.frameTimer += dt.asSeconds();

			if (bird.frameTimer > 0.1f) {
				bird.frameTimer = 0.0f;
				bird.currentFrame++;
			}

			if (bird.currentFrame > 2) {
				bird.currentFrame = 0;
			}

			if (bird.currentFrame == 0) {
				bird.shape.setTexture(&bird.texUp);
			}
			else if (bird.currentFrame == 1) {
				bird.shape.setTexture(&bird.texMid);
			}
			else{
				bird.shape.setTexture(&bird.texDown);
			}


			float bgspeed = -0.5f;
			bgSprite1.move({ bgspeed,0 });
			bgSprite2.move({ bgspeed,0 });

			float scaledWidth = bgTexture.getSize().x * bgSprite1.getScale().x;

			if (bgSprite1.getPosition().x < -scaledWidth) {
				bgSprite1.setPosition({ bgSprite2.getPosition().x + scaledWidth,0.0f });
			}

			if (bgSprite2.getPosition().x < -scaledWidth) {
				bgSprite2.setPosition({ bgSprite1.getPosition().x + scaledWidth,0.0f });
			}

			//1. Gravity increases the falling 
			bird.velocity += gravity;
			//changing position as the speed changes
			bird.shape.move({ 0,bird.velocity });

			bird.shape.setRotation(sf::degrees(bird.velocity * 3.0f));

			//Score Counting

			if (TopPipe.getPosition().x < 100 && !scorecounted) {
				score++;
				scorecounted = true;

			}
			//if the bird falls down

			//2.Pipe Movement
			TopPipe.move({ pipeSpeed,0 });
			BottomPipe.move({ pipeSpeed,0 });


			//If one pipe passes the left window
			if (TopPipe.getPosition().x < -60.0f) {
				//Start is always the right window
				float StartX = 800.0f;

				//Generate a Random Y for the Gap Start
				//when a gap starts do the math and subtract -600 from the gap start position of gap to get the start position of the top pipe
				//add GapHeight to generate the bottompipe start position
				float gapY = (std::rand() % 300) + 100.0f;

				TopPipe.setPosition({ StartX,gapY });

				BottomPipe.setPosition({ StartX,gapY + Gap });
				scorecounted = false;
			}

			//3.Collision Detection
			//get invisible box around them
			sf::FloatRect birdbound = bird.shape.getGlobalBounds();
			sf::FloatRect toppipebound = TopPipe.getGlobalBounds();
			sf::FloatRect bottompipebound = BottomPipe.getGlobalBounds();

			float skinwidth = 5.0f;
			birdbound.position.x += skinwidth;
			birdbound.position.y += skinwidth;
			birdbound.size.x -= (skinwidth * 2);
			birdbound.size.y -= (skinwidth * 2);

			//check if they touch
			if (birdbound.findIntersection(toppipebound) || birdbound.findIntersection(bottompipebound) || bird.shape.getPosition().y > 600) {
				if (!isGameOver) hitsnd.play();
				isGameOver = true;

				//-Saving High Score-
				if (score > highScore) {
					highScore = score; //update if bigger

					highScoreText.setString("Best: " + std::to_string(highScore));

					//Save to File
					std::ofstream writeFile("highscore.txt");
					writeFile << highScore;
					writeFile.close();
				}

				music.pause();
			}
		}

		

		window.clear(sf::Color::Black);

		//drawing the background here so it is the bottom layer,everything will be drawn on top of here
		window.draw(bgSprite1);
		window.draw(bgSprite2);
		window.draw(TopPipe);
		window.draw(BottomPipe);
		window.draw(bird.shape);
		
		//--SCORE DISPLAY --
		if (!isGameOver) {
			std::string ScoreStr = std::to_string(score); //convert int to string
			//1.Calculate the center position
			//we assume the images are roughly 24 pixels wide
			float numberWidth = numberTextures[0].getSize().x;
			float totalWidth = numberWidth * ScoreStr.length();
			float startX = (width / 2.0f) - (totalWidth / 2.0f); //centers the character 

			for (int i = 0; i < ScoreStr.length(); i++) {
				char digitChar = ScoreStr[i];

				int digit = digitChar - '0';

				sf::Sprite digitSprite(numberTextures[digit]);

				//position it and show
				digitSprite.setPosition({ startX + (i * numberWidth) , 50.0f });

				window.draw(digitSprite);
			}
		}

		if (isGameOver) {
			window.draw(gameOverSprite);
			window.draw(highScoreText);
			music.pause();
		}

		window.display();

	}
}