#ifndef BODY_HPP
#define BODY_HPP

#include <string>

#include "Entity.hpp"

/*
	Example body [ 
	'	', '/', 'x', '>', ' ',
	'=, '=', 'x', '=', '>',
	'	', '\', 'x', '>', ' '
	]

	This will have a height of 3 and a width of 5.
	Result on terminal

			;-----;
			| /x> |
			|==x=>|
			| \x> |
			;-----;

*/

class Body: virtual public Entity
{
public:
	Body(const std::string body, int width, int height);
	Body(const Body &other);

	int getWidth() const;
	int getHeight() const;
	const std::string &getBody() const;
	void setBody(const std::string b, int w, int h);

private:
	Body();
	std::string _body;
	int _width;
	int _height;
};

#endif
