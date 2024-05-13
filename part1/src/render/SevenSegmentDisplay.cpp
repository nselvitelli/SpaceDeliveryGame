#include "render/SevenSegmentDisplay.hpp"

#include <iostream>

SevenSegmentDisplay::SevenSegmentDisplay(std::vector<std::string> digitTextureNames,
                                        GUI* gui,
                                        glm::vec3 screenPosition,
                                        glm::vec2 dimensions,
                                        int numDigits, 
                                        int initialNumber)
                                        : m_digitTextureNames(digitTextureNames),
                                          m_numDigits(numDigits),
                                          m_displayNumber(initialNumber),
                                          m_targetNumber(initialNumber) {

    int digitWidth = dimensions.x / numDigits;

    for(int i = 0; i < numDigits; i++) {
        Object* test = new Object();
        test->MakeTexturedQuad(m_digitTextureNames[0]);

        GUINode* guiNode = new GUINode(test);
        guiNode->ShowOnScreen(false);
        gui->AddNode(guiNode); // add to GUI first to update width and height

        int x = (int)screenPosition.x + (digitWidth * (numDigits - 1 - i));

        guiNode->PlaceOnScreen(x, screenPosition.y, screenPosition.z);
        guiNode->Resize(digitWidth, dimensions.y);

        m_digitNodes.push_back(guiNode);
    }

    SetNumber(initialNumber);
}

SevenSegmentDisplay::~SevenSegmentDisplay() {

}

void SevenSegmentDisplay::Increment(int number) {
    m_targetNumber = m_displayNumber + number;
}

void SevenSegmentDisplay::Update() {
    if(m_displayNumber < m_targetNumber) {
        SetNumber(m_displayNumber + 1);
    }
}

void SevenSegmentDisplay::SetNumber(int number) {
    m_displayNumber = number;

    for(int index = 0; index < m_numDigits; index++) {
        if(number > 0) {
            int digit = number % 10;
            m_digitNodes[index]->SetTextureFromLibrary(m_digitTextureNames[digit]);
            m_digitNodes[index]->ShowOnScreen();
            number = number / 10;
        }
        else {
            m_digitNodes[index]->ShowOnScreen(false);
        }
    }
}