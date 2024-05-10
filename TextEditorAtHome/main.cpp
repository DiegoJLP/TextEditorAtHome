#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

using namespace std;
using namespace sf;

class TextBox {
public:
    TextBox(int size, Color color, bool sel, Vector2f pos) {
        textbox.setCharacterSize(size);
        textbox.setFillColor(color);
        isSelected = sel;
        this->setPosition(pos);
        if (sel) {
            textbox.setString("_");
        }
        else {
            textbox.setString("");
        }
    }

    void setFont(Font& font) {
        textbox.setFont(font);
    }

    void setPosition(Vector2f pos) {
        textbox.setPosition(pos);
    }

    void setLimit(bool ToF, int lim) {
        hasLimit = ToF;
        limit = lim;
    }
    void moveCursor(int direction) {
        int cursorPosition = text.tellp();

        if (direction == Keyboard::Left && cursorPosition > 0) {
            cursorPosition--;

        }
        else if (direction == Keyboard::Right && cursorPosition < text.str().length()) {
            cursorPosition++;
        }

        text.seekp(cursorPosition);

        string displayedText = text.str();
        displayedText.insert(cursorPosition, "_");
        textbox.setString(displayedText);
    }

    void setSelected(bool sel) {
        isSelected = sel;
        if (!sel)
        {
            string t = text.str();
            string newT = "";
            for (int i = 0; i < t.length() - 1; i++)
            {
                newT += t[i];
            }
            textbox.setString(newT);
        }
    }

    string getText() {
        return text.str();
    }

    void drawTo(RenderWindow& window) {
        window.draw(textbox);
    }


    void typedOn(Event input) {
        if (isSelected)
        {
            int charTyped = input.text.unicode;
            if (charTyped < 128)
            {
                if (hasLimit)
                {
                    if (text.str().length() <= limit) {
                        inputLogic(charTyped);
                    }
                    else if (text.str().length() > limit && charTyped == DELETE_KEY) {
                        deleteLastChar();
                    }
                }

                else {
                    inputLogic(charTyped);
                }
            }
        }
    }

private:
    Text textbox;
    ostringstream text;
    int limit;
    bool hasLimit;
    bool isSelected = false;

    void inputLogic(int charTyped) {
        if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
            text << static_cast<char>(charTyped);
        }
        else if (charTyped == DELETE_KEY) {
            if (text.str().length() > 0)
            {
                deleteLastChar();
            }
            textbox.setString(text.str() + "_");
        }
        textbox.setString(text.str() + "_");
    }

    void deleteLastChar() {
        string t = text.str();
        string newT = "";
        for (int i = 0; i < t.length() - 1; i++)
        {
            newT += t[i];
        }
        text.str("");
        text << newT;
        textbox.setString(text.str());
    }

};


int main() {
    RenderWindow window;
    Vector2i centerWindow((VideoMode::getDesktopMode().width / 2) - 445, (VideoMode::getDesktopMode().height / 2) - 480);
    window.create(VideoMode(900, 900), "Editor de Texto", Style::Titlebar | Style::Close);
    window.setPosition(centerWindow);
    window.setKeyRepeatEnabled(true);
    Font fuente;
    if (!fuente.loadFromFile("Oswald-VariableFont_wght.ttf"))
    {
        throw "NO PUEDO ABRIR LA FUENTEEEEE";
    }
    TextBox textbox1(15, Color::White, true, { 100,100 });
    textbox1.setFont(fuente);
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed:
                    window.close();
                case Event::TextEntered:
                    textbox1.typedOn(event);

                case Event::KeyPressed:
                    if (event.key.code == Keyboard::Left || event.key.code == Keyboard::Right) {
                        int direction = event.key.code;
                        textbox1.moveCursor(direction);

                    }
                    break;
                default:
                    break;
            }
        }
        window.clear();
        textbox1.drawTo(window);
        window.display();
    }
    return 0;
}
