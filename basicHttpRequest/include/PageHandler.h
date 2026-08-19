#include <Adafruit_ST7735Keyboard.h>

#define LEFT "a"
#define UP "w"
#define RIGHT "d"
#define DOWN "s"
#define SELECT " "

typedef enum
{
    MENU_PAGE = 0,
    SUMMARY_PAGE = 1,
    POSITIONS_PAGE = 2,
} Page;

String in;

void setCreds(Adafruit_ST7735Keyboard *tft, Preferences *preferences)
{
    if ((*preferences).getString("ssid").isEmpty() or (*preferences).getString("pass").isEmpty())
    {
        (*tft).fillScreen(ST7735_BLACK);
        (*tft).putKeyboard(111, false);
        (*tft).setCursor(5, 15);
        (*tft).setFont(&Keyboard);
        (*tft).println("ssid:");
        (*tft).setFont();

        String mySsid = (*tft).takeInput(5, 30);
        (*preferences).putString("ssid", mySsid);

        (*tft).setCursor(5, 45);
        (*tft).setFont(&Keyboard);
        (*tft).println("Password:");
        (*tft).setFont();
        String myPass = (*tft).takeInput(5, 60);
        (*preferences).putString("pass", myPass);
    }

    if ((*preferences).getString("id").isEmpty() or (*preferences).getString("key").isEmpty())
    {
        (*tft).fillScreen(ST7735_BLACK);
        (*tft).putKeyboard(111, false);
        (*tft).setCursor(5, 15);
        (*tft).setFont(&Keyboard);
        (*tft).println("API ID:");
        (*tft).setFont();

        String myId = (*tft).takeInput(5, 30);
        (*preferences).putString("id", myId);

        (*tft).setCursor(5, 45);
        (*tft).setFont(&Keyboard);
        (*tft).println("API Key:");
        (*tft).setFont();
        String myKey = (*tft).takeInput(5, 60);
        (*preferences).putString("key", myKey);
    }
}

Page MenuPageHandler(Adafruit_ST7735Keyboard *tft, Page *previousPage)
{
    *previousPage = MENU_PAGE;
    MenuSelection menuSelection = SUMMARY;
    (*tft).printMenu(menuSelection);
    while (true)
    {
        in = Serial.readString();
        if (in == UP)
        {
            // Change the highlighted button
            menuSelection = (menuSelection == 0) ? OPEN_POSITIONS : MenuSelection((menuSelection - 1) % 2);
            (*tft).printMenu(menuSelection);
        }
        else if (in == DOWN)
        {
            // Change the highlighted button
            menuSelection = MenuSelection((menuSelection + 1) % 2);
            (*tft).printMenu(menuSelection);
        }
        else if (in == SELECT)
        {
            // Return which button is highlighted and exit this page handler
            switch (menuSelection)
            {
            case SUMMARY:
                return SUMMARY_PAGE;
            case OPEN_POSITIONS:
                return POSITIONS_PAGE;
            }
        }
    }
}

Page SummaryPageHandler(Adafruit_ST7735Keyboard *tft, Page *previousPage, Summary summary)
{
    *previousPage = SUMMARY_PAGE;
    SummarySelection summarySelection = PIES;
    (*tft).printSummary(summary, summarySelection);
    while (true)
    {
        in = Serial.readString();
        if (in == LEFT)
        {
            // Change the highlighted button
            summarySelection = (summarySelection == 0) ? PIES : SummarySelection((summarySelection - 1) % 2);
            (*tft).printSummary(summary, summarySelection);
        }
        else if (in == RIGHT)
        {
            // Change the highlighted button
            summarySelection = SummarySelection((summarySelection + 1) % 2);
            (*tft).printSummary(summary, summarySelection);
        }
        else if (in == SELECT)
        {
            // Return which button is highlighted and exit this page handler
            switch (summarySelection)
            {
            case PIES:
                Serial.println("Pies WIP");
                break;
            case MENU_SUMMARY:
                return MENU_PAGE;
            }
        }
    }
}

Positions *PositionsPageFlipLeft(Positions *positions, Positions *currentPositions)
{
    if (positions->count - currentPositions->count >= 7) // If there are still pages on the left
    {
        for (int i = 0; i < 7; i++)
            currentPositions = currentPositions->prevPos;
    }
    else // If we're on the first page, go to the last page
    {
        for (int i = 0; i < (positions->count / 7) * 7; i++)
            currentPositions = currentPositions->nextPos;
    }
    return currentPositions;
}

Positions *PositionsPageFlipRight(Positions *positions, Positions *currentPositions)
{
    if (currentPositions->count >= 7) // If there are still pages on the right
    {
        for (int i = 0; i < 7; i++)
            currentPositions = currentPositions->nextPos;
    }
    else // If we're at the end of the pages, go back to the first page
    {
        int moveBy = positions->count - currentPositions->count;
        for (int i = 0; i < moveBy; i++)
            currentPositions = currentPositions->prevPos;
    }
    return currentPositions;
}

Page BuySellPositionHandler(Adafruit_ST7735Keyboard *tft, WiFiClass *WiFi, Position position, bool buy)
{
    (*tft).printBuySellMenu(buy);
    (*tft).putKeypad(98);
    while (true)
    {
        in = Serial.readString();
        if (in == SELECT)
            return POSITIONS_PAGE;
    }
}

Page PositionPageHandler(Adafruit_ST7735Keyboard *tft, WiFiClass *WiFi, Positions *currentPositions, int positionsSize)
{
    PositionSelection positionSelection = BACK;
    (*tft).printPosition(currentPositions->currentPos, currentPositions->count, positionsSize, positionSelection);
    while (true)
    {
        in = Serial.readString();
        if (in == LEFT)
        {
            if (currentPositions->count == positionsSize)
            {
                for (int i = 0; i < positionsSize - 1; i++)
                    currentPositions = currentPositions->nextPos;
            }
            else
                currentPositions = currentPositions->prevPos;
            positionSelection = BACK;
            (*tft).printPosition(currentPositions->currentPos, currentPositions->count, positionsSize, positionSelection);
        }
        else if (in == RIGHT)
        {
            if (currentPositions->count == 1)
            {
                for (int i = 0; i < positionsSize - 1; i++)
                    currentPositions = currentPositions->prevPos;
            }
            else
                currentPositions = currentPositions->nextPos;
            positionSelection = BACK;
            (*tft).printPosition(currentPositions->currentPos, currentPositions->count, positionsSize, positionSelection);
        }
        else if (in == UP)
        {
            positionSelection = positionSelection == BUY ? BACK : PositionSelection(positionSelection - 1);
            (*tft).printPosition(currentPositions->currentPos, currentPositions->count, positionsSize, positionSelection);
        }
        else if (in == DOWN)
        {
            positionSelection = positionSelection == BACK ? BUY : PositionSelection(positionSelection + 1);
            (*tft).printPosition(currentPositions->currentPos, currentPositions->count, positionsSize, positionSelection);
        }
        else if (in == SELECT)
        {
            switch (positionSelection)
            {
            case BACK:
                return POSITIONS_PAGE;
            case BUY:
                BuySellPositionHandler(tft, WiFi, currentPositions->currentPos, true);
                (*tft).printPosition(currentPositions->currentPos, currentPositions->count, positionsSize, positionSelection);
                break;
            case SELL:
                BuySellPositionHandler(tft, WiFi, currentPositions->currentPos, false);
                (*tft).printPosition(currentPositions->currentPos, currentPositions->count, positionsSize, positionSelection);
                break;
            }
        }
    }
}

Page PositionsPageHandler(Adafruit_ST7735Keyboard *tft, WiFiClass *WiFi, Page *previousPage, Positions *positions)
{
    *previousPage = POSITIONS_PAGE;
    int positionsSelection = 0;
    Positions *currentPositions = positions;
    (*tft).printPositions(currentPositions, positions->count, positionsSelection);
    while (true)
    {
        in = Serial.readString();
        if (in == LEFT)
        {
            // Reset the indicator for the individual position
            positionsSelection = 0;
            currentPositions = PositionsPageFlipLeft(positions, currentPositions);
            (*tft).printPositions(currentPositions, positions->count, positionsSelection);
        }
        else if (in == RIGHT)
        {
            // Reset the indicator for the individual position
            positionsSelection = 0;
            currentPositions = PositionsPageFlipRight(positions, currentPositions);
            (*tft).printPositions(currentPositions, positions->count, positionsSelection);
        }
        else if (in == UP)
        {
            // Check if we should be on a button or a position
            switch (positionsSelection)
            {
            case 0:
                positionsSelection = 7;
                break;
            case 7:
                if (currentPositions->count < 7)
                    positionsSelection = currentPositions->count - 1;
                else
                    positionsSelection--;
                break;
            default:
                positionsSelection--;
            }
            (*tft).printPositions(currentPositions, positions->count, positionsSelection);
        }
        else if (in == DOWN)
        {
            // Check if we should be on a button or a position
            if (positionsSelection == 7)
                positionsSelection = 0;
            else if (positionsSelection == currentPositions->count - 1)
                positionsSelection = 7;
            else
                positionsSelection++;
            (*tft).printPositions(currentPositions, positions->count, positionsSelection);
        }
        else if (in == SELECT)
        {
            // If we need to go to a different page, exit the function, otherwise put us in a detailed view of a position
            switch (positionsSelection)
            {
            case 7:
                return MENU_PAGE;
            default:
                // Move to the highlighted position
                for (int i = 0; i < positionsSelection; i++)
                    currentPositions = currentPositions->nextPos;
                PositionPageHandler(tft, WiFi, currentPositions, positions->count);
                // Move back to one of the positions that would be at the start of a page
                int moveBy = positions->count - (((positions->count - currentPositions->count) / 7) * 7) - currentPositions->count;
                positionsSelection = 0;
                for (int i = 0; i < moveBy; i++)
                    currentPositions = currentPositions->prevPos;
                (*tft).printPositions(currentPositions, positions->count, positionsSelection);
            }
        }
    }
}