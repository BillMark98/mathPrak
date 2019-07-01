#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    // sf::CircleShape shape(100.f);
    // shape.setFillColor(sf::Color::Green);
    // sf::RenderWindow window1(sf::VideoMode(800,600),"Window2");
    sf::Font font;
    if(!font.loadFromFile("arial.ttf"))
    {
        cout << "cant read file\n";
        exit(1);
    }
    sf::Text text;
    text.setFont(font);
    text.setString("Hello there\n");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Blue);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::GainedFocus)
            {
                cout << "window 1 gained focus\n";
            }    
            if (event.type == sf::Event::LostFocus)
            {
                cout << "window 1 lost focus\n";
            }    
            
        }

        window.clear(sf::Color::White);
        // window.draw(shape);
        window.draw(text);
        window.display();
    }

// the second loop wont be entered until the first one is ended
    // while (window1.isOpen())
    // {
    //     sf::Event event;
    //     while (window1.pollEvent(event))
    //     {
    //         if (event.type == sf::Event::Closed)
    //             window1.close();
    //         if (event.type == sf::Event::GainedFocus)
    //         {
    //             cout << "window 2 gained focus\n";
    //         }    
    //         if (event.type == sf::Event::LostFocus)
    //         {
    //             cout << "window 2 lost focus\n";
    //         }     
            
    //     }

    //     window1.clear();
    //     // window.draw(shape);
    //     window1.display();
    // }




    return 0;
}
