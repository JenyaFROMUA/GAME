


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;

const int H = 12;
const int W = 40;


String TileMap[H] = {

	"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
	"B                                      B",
	"B                      B    B          B",
	"B                             B        B",
	"B                             B        B",
	"B                             BBB      B",
	"B                               B     BB",
	"BBB                             B    B B",
	"B                               B  B   B",
	"B              B    B                 BB",
	"B    B         BB         BB         BBB",
	"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",

}; 

class Vector{
public:
	float x,y;
	Vector(){x=y=0;}
	Vector(float x, float y) : x(x), y(y){}
	Vector(float xa, float ya, float xb, float yb) : x(xb-xa) , y(yb-ya){}
	typedef Vector myt;
	void Set(float X, float Y){
		x = X;
		y = Y;
	}
	void Offset(float dx, float dy){
		x += dx;
		y += dy;
	}
	myt& operator= (const myt& t){
		Set(t.x, t.y);
		return *this;
	}
	myt operator + (const myt& t){
		return myt(x+t.x, y+t.y);
	}
	myt& operator += (const myt& t){
		x += t.x;
		y += t.y;
		return *this;
	}
	myt operator - (const myt& t){
		return myt(x-t.x, y-t.y);
	}
	myt& operator -= (const myt& t){
		x -= t.x;
		y -= t.y;
		return *this;
	}
	myt operator* (const float &f){
		return myt(x*f, y*f);
	}
	myt& operator *= (const float &f){
		x *= f;
		y *= f;
	}
	friend Vector operator *(const float &f, const Vector& t){
		return Vector(f*t.x, f*t.y);
	}
	float operator * (const myt& t){
		return x*t.x + y*t.y;
	}
	myt operator/ (const float &t){
		return myt(x/t,y/t);
	}
	myt& operator /= (const float& t){
		x /= t;
		y /= t;
		return *this;
	}
};

class Animation : public Drawable{
public:
	Animation(){}
	Animation(const FloatRect& texrect, int frames, const Texture& t) : rect(texrect.left, texrect.top, texrect.width/frames, texrect.height){
		tex = new Texture(t);
		sprite = new Sprite(*tex, rect);
		currentframe = 1;
		Frames = frames;
	}

	void Next(bool reverse = false){
		if(currentframe++ != Frames){
			rect.left += abs(rect.width);
		}
		else {
			rect.left -= abs(rect.width)*Frames - 40;
			currentframe = 1;
		}
		if(reverse && rect.width > 0 || (!reverse && rect.width < 0)){
			rect.width *= -1;
			if(rect.width < 0) rect.left += abs(rect.width);
			else rect.left -= rect.width;
		}
		sprite->setTextureRect(rect);
	}
	void Previous(){
		if(currentframe-- > Frames) {
			rect.left += rect.width;
		}
		else{
			currentframe = Frames;
			rect.left += rect.width*Frames;
		}
		sprite->setTextureRect(rect);
	}
	void Move(float dx, float dy){
		sprite->move(dx, dy);
	}
	virtual void draw(sf::RenderTarget& window, sf::RenderStates states)const{
		window.draw(*sprite, states);
	}
private:
	int Frames;
	int currentframe;
	IntRect rect;
	sf::Texture *tex;
	Sprite *sprite;
};
class Player: public Drawable{

protected:
	FloatRect myrect;
	Animation sprite;
	float mytime;
public:
	Player(){}
	Player(float x, float y, const Texture& tex, const FloatRect& texrect, int frames) : sprite(texrect, frames, tex), 
		myrect(x,y, texrect.width/frames, texrect.height), mytime(0){
	}
	

	void update(float time)
	{
		
		////////////////////////
		if(Keyboard::isKeyPressed(Keyboard::Left))
		{
			updateAnimation(time,true);
			Move(-0.1*time,0);
		}
		////////////////////////////////////////////////////////////
		if(Keyboard::isKeyPressed(Keyboard::Right))
		{
			updateAnimation(time);
			Move(0.1*time,0);
		}

		if(Keyboard::isKeyPressed(Keyboard::Up))
		{
			Move(0,-0.1*time);
		}

		if(Keyboard::isKeyPressed(Keyboard::Down))
		{
			Move(0,+0.1*time);

		}

	}
	void updateAnimation(float dt, bool r = false){
		mytime+= dt;
		if(mytime > 200){
			mytime = 0;
			sprite.Next(r);
		}
	}
	Vector getPoint()const{
		return Vector(myrect.left, myrect.top);
	}
	void Move(float dx, float dy){
		sprite.Move(dx, dy);
		myrect.left += dx;
		myrect.top += dy;
	}
	void draw(sf::RenderTarget& target, sf::RenderStates states)const{
		target.draw(sprite);
	}

};
class Enemy: public Player{
	private:
	Vector movement;
public:
	Enemy(){}
	Enemy(float x, float y, const Texture& tex, const FloatRect& texrect, int frames):Player(x,y,tex,texrect,frames){}
	 
	void update(float dt){
		Move(movement.x, movement.y);
		updateAnimation(dt);
	}
	void setArrow(float x, float y){
		movement.Set(x-myrect.left, y - myrect.top);
		movement /= 1000;
	}
	void setArrow(const Player& player){
		movement.Set(player.getPoint().x - myrect.left , player.getPoint().y - myrect.top);
		movement /= 10;
	}

};


int main()
{
	RenderWindow window(VideoMode(1280, 400), "SWAG");

	Texture t, b, back;

	t.loadFromFile("fang.png");
	b.loadFromFile("starwars.png");
	back.loadFromFile("bg.jpg");
	Sprite background(back, IntRect(0,0,1280,400));

	Player player(50,10, t, FloatRect(0,244,240,50), 6);
	Enemy enemy(200,200, b, FloatRect(273,563,220, 64), 5);
	Enemy enemy2(100, 300, b,FloatRect(327,552, 233, 69), 5);
	SoundBuffer buffer;
	buffer.loadFromFile("Jump.ogg");
	Sound sound(buffer);

    Music music;
    music.openFromFile("1.ogg");
    music.play();
	Clock clock;

	RectangleShape rectangle(Vector2f(32,32));

	while(window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time /= 800;
		Event event;
		while(window.pollEvent(event))
		{

			if(event.type == Event::Closed )
				window.close();
		}
	
		player.update(time);
		enemy.setArrow(player);
		enemy2.setArrow(player);
		enemy.update(time);
		enemy2.update(time);
		window.clear(Color::White);

		/*for (int i=0; i<H; i++)
			for (int j=0; j<W ; j++)
			{ 
				if (TileMap[i][j]=='B') rectangle.setFillColor(Color::Black);

				if (TileMap[i][j]=='1')  rectangle.setFillColor(Color::Green);

				if (TileMap[i][j]==' ') continue;

				rectangle.setPosition(j*32,i*32) ; 
				window.draw(rectangle);
			}*/
			window.draw(enemy2);
			window.draw(background);
			window.draw(enemy);
			window.draw(player);
			window.display();
	}

	return 0;
}


