# Project docs

## jsonGetters.h

This header provides a class called extractData. The project uses the cJSON library to parse the response from APIs but since there are multiple responses and multiple values are needed from them, the cJSON library by itself doesn't give the cleanest code since it uses a linked-list style approach the code would end up having very long lines full of cjsonObject->next->next->child->next.......

This header file solves that with methods that abstract away from those long lines.

### Imported includes:

- cJSON.h

### Self-defined includes:

- None.

### Currency enum

This enum has a corresponding value for every currency code that the Trading212 API may return. This makes it easier to print currency symbols to the display since the display is not able to have a glyph for multi-character symbols such as €. For this reason, I'll have to have each currency character represented by a different character in a font file for the display, using numbers for the characters makes this enum very useful!

### Functions:

- currencySymbol(currCode)
    - This function converts the currency code returned by the Trading212 API, which is a String, into the currency enum so that the currency symbol can be easily printed to the display.
    - Receives: currCode (String)
    - Outputs: Currency (enum)

### Classes:

- ExtractData
    - Public:
        - Constructors:
            - There are 3 ways to create an object of this type, the default way, which creates an empty object. This is used to initialise an object of this type before any calls to APIs are made so that the objects isn't re-initialised every time that an API response is received. The other two ways take in either a char*, which is parsed with the cJSON library, or a cJSON*, which has already been parsed.
        - getJson()
            - Returns: jsonData (the cJSON* of the object)
        - All of the other functions are essentially crawling the cJSON* attribute of this object to return any value from the json returned by an API. Since the API can return different things, such as account data, or positions data, the methods for each are denoted by comments inside the header file.
    - Private:
        - jsonData: the cJSON* which points to all of the data returned by the API.

## trading212Data.h

This header contains functions/classes that call the Trading212 API and format the response in a way that is easier to understand and work with.

### Imported includes:

- WiFi.h
- HTTPClient.h

### Self-defined includes:

- jsonGetters.h

### Classes:

- Summary
    - An instance of this class is used to store the important values returned by the Trading212 summary endpoint
    - Public:
        - Constructors:
            - One constructor creates an empty object, this is used to initalise a Summary in a place in the code such that it will not be re-initialised.
            - The other constructor receives the base64 encoded Trading212 credentials and a pointer to a WiFiClass object. These are used to make calls to the Trading212 API and the response is stored in the attributes of this class.
        - All of the other methods simply return the private attributes of this class.
    - Private:
        - httpCode: the integer value code returned by the http call to the Trading212 summary API endpoint
        - payload: the string returned by the http call to the Trading212 summary API endpoint
        - curr: the currency code string for the wallet currency of the user
        - totalVal: a float, the total value of the user's account
        - availableToTrade: a float, the total cash available to be traded for the user
        - reservedForOrders: a float, the total cash in a user's account that has already been reserved for orders
        - totalCost: a float, the total value that a user has paid for the positions that they currently own
        - unrealisedProfit: a float, amount that a user would lose or gain if they sold all of their positions in this moment. This value can be positive or negative depending on if the user would gain or lose money.

- Position
    - Instances of this class are used to store important values about each position that is returned by the open positions endpoint of the Trading212 API
    - Public:
        - Constructors:
            - One constructor creates an empty object which is used to avoid re-initialising instances
            - The other constructor takes in values for each attribute and assigns them accordingly
        - All of the other methods return the private attributes of this class.
    - Private:
        - name: the string that contains the name of the position
        - walletCurr: a string value for the wallet currency code of the user
        - instrumentCurrency: a string value for the instrument currency code of the position
        - paidPerShare: a float value for the average price paid per share of this position in instrument currency
        - currShareVal: a float value for the current value of one share of this position in instrument currency
        - totalMarketVal: a float value for the current market value in the wallet currency of the user
        - unrealisedProfit: a float value for how much money you'd gain or lose if you sold all shares of this position this instant in wallet currency
        - sharesAvail: a float value for the number of shares currently available for trading

### Structs:

- Positions
    - Attributes:
        - currentPos: in a linked list, this would be the data of the current node. It's of Position type
        - nextPos and prevPos: pointers to the Positions after and before the current node
        - count: an integer representing how many positions are after the current one, including the current position
    - Methods:
        - Constructors: one empty constructor and one that takes a value for currentPos, prevPos, and count.

### Functions:

- makePositions(payloadJson, created)
    - This function creates the Positions linked list
    - Receives: a cJSON* pointer that contains the json returned by the positions Trading212 endpoint, and a pointer to a boolean variable that says whether or not positions has been created yet, this is used to not create multiple instances of positions as it takes up a lot of memory.
    - Outputs: a Positions pointer to the head of the Positions linked list.
- getPositions(encoded, WiFi)
    - This function sends a request to the Trading212 positions API endpoint and returns the json that it receives
    - Receives: base64 encoded credentials, and a pointer to the WiFiClass
    - Outputs: a cJSON pointer that contains the data returned by the positions Trading212 endpoint
- freePositions(positions, created)
    - This function frees the memory that is taken up by the Positions linked list since that memory is assigned using the *new* keyword.
    - Receives: a pointer to the positions linked list, and a boolean that tells the function whether the linked list exists or not
    - Outputs: nothing.

## keyboardFont.h

This header contains the GFX font created with the [Adafruit GFX font customiser](https://tchapi.github.io/Adafruit-GFX-Font-Customiser/) which is used to print the characters on on-screen keyboard.

## currencySymbols.h

This header contains the GFX font created with the [Adafruit GFX font customiser](https://tchapi.github.io/Adafruit-GFX-Font-Customiser/) which is used to print currency symbols that don't exist in the default Adafruit_GFX fonts.

## Adafruit_ST7735Ext.h

This header extends the Adafruit_ST7735 class, adding more specific methods which allow easy printing for things such as the Trading212 logo and pages for the device.

### Imported includes:

- Adafruit_ST7735

### Self-defined includes:

- trading212Data.h
- keyboardFont.h
- currencySymbols.h

### enums:

- MenuSelection
    - This is used to keep track of which button is selected in the main boot-up menu
- SummarySelection
    - This is used to keep track of which button is selected in the summary page
- PositionSelection
    - This is used to keep track of which button is selected in the individual position page

### Classes:

- Adafruit_ST7735Ext
    - This is an extention of the Adafruit_ST7735 class by Adafruit and adds methods to print pages from this project, among other things like the Trading212 logo
    - Public:
        - movingText(toWrite, y)
            - This prints some text at a certain y-level on the display, it's padded by 10 pixels on the left and right and if it's too long, the text wipes across the display. This function may not be used due to the slow refresh rate of the display.
            - Receives: a String which is to be written, and an integer for the y-level at which to write the String.
            - Outputs: nothing.
        - logo()
            - This draws the Trading212 logo with triangles on the display and writes Trading212 in large font below on the display.
            - Receives: nothing.
            - Outputs: nothing.
        - setFontKeepSize()
            - This changes the font without changing the y-level of the cursor, which the regular Adafruit_ST7735 setFont function cannot do. This is used when printing a currency symbol followed by an amount of money without having to setCursor as well as setFont between each print.
            - Receives: nothing.
            - Outputs: nothing.
        - printMoney(curr, amount)
            - This prints a currency symbol followed by an amount of money to the display, it uses setFontKeepSize to do this.
            - Receives: a currency enum, and a float type amount of money
            - Outputs: nothing.
        - printCentreLeftAlign(toPrint, y)
            - This prints some text left-aligned, padded by 5 pixels either side to the display. If the text is too long to be on one line, it's written across multiple lines until there is no more text to write.
            - Receives: a String toPrint, an integer y which is the y-level at which to print the text.
            - Outputs: nothing.
        - printUnderlineDefaultFont(toPrint, colour)
            - Prints some text to the display, in the default GFX font and underlines it 
            - Receives: a String toPrint, a uint16_t colour which to make the text and line.
            - Outputs: nothing.
        - printCentered(toPrint)
            - Prints some text to the display centered, used to show the page number
            - Receives: a String toPrint
            - Outputs: nothing.
        - printSummary(aSummary, summarySelection)
            - Prints the summary page to the display
            - Receives: a Summary, a SummarySelection which represents the button to be highlighted
            - Outputs: nothing.
        - printPageNum(pageNum, totalPages)
            - Prints the current page number and the total pages at the bottom of the display
            - Receives: the current page number, and the total pages
            - Outputs: nothing.
        - printPositions(positions, totalCount, select)
            - Prints one page of open positions to the display, with a page number at the bottom, highlights the selected position/button
            - Receives: a Positions pointer to the current node in the positions linked list, an integer for the total number of positions, and an integer for which position/button is selected (using an integer instead of enum as it makes it easier to select positions)
            - Outputs: nothing.
        - printPosition(position, positionCount, totalCount, select)
            - Prints one position in detailed view to the display
            - Receives: the Position that should be shown, an integer for the number of positions remaining (including the current position), an integer for the total amount of positions received from the Trading212 json response, a PositionSelect enum to represent which button on the page is selected
            - Outputs: nothing.
        - printMenu(select)
            - Prints the main menu to the display
            - Receives: a MenuSelect to tell which button is selected
            - Outputs: nothing.

## Adafruit_ST7735Keyboard.h

This header extends the previously-defined Adafruit_ST7735Ext class in order to give it on-screen keyboard functionality.

### Imported includes:

- Fonts/PicoPixel.h (an Adafruit GFX default font)

### Self-defined includes:

- Adafruit_ST7735Ext.h

### Defines

These are all characters that are unused in the keyboard so they can be safely used to represent special buttons on the keyboard. This allows things such as switch cases to be used for characters returned from the keyboard.

- SHIFT: **/**
- BACKSPACE: __*__
- ENTER: **~**

### keyboardDirection enum

This is used to make switching based on the keyboard direction more natural to write/work with

### Classes

- Adafruit_ST7735Keyboard
    - This is an extension of the Adafruit_ST7735Ext class, mainly to give it the ability to print a keyboard on the display and received user-input Strings
    - Public:
        - takeInput(x, y)
            - This calls upon other methods in order to:
                - Print the keyboard to the display
                - Print what the user has input so far to the display
                - Change what key on the keyboard is highlighted depending on user input
                - Return the String that the user has input
            - Receives:
                - Integers x and y, these dictate where the user-input word will be displayed as the user is writing it
            - Outputs:
                - The String that the user has entered
        - drawBackspace(x, y, colour)
            - This prints a backspace (left facing arrow) to the display
            - Receives: integers x and y, and uint16_t colour, which dictate where the backspace should be drawn and in which colour
            - Outputs: nothing.
        - drawEnter(x, y, colour)
            - This prints an enter key (right-angled arrow pointing left with tail pointing upwards) to the display
            - Receives: integers x and y, and uint16_t colour, which dictate where the enter key should be drawn and in which colour
            - Outputs: nothing.
        - putKeyboard(height, capital)
            - This prints a keyboard to the display
            - Receives: integer height which dictates at which y-level to place the keyboard, and boolean capital which dictates whether or not the keys are capitalised
            - Outputs: nothing.
        - selectKey(key)
            - This highlights one key on the keyboard
            - Receives: a char for which key is to be highlighted
            - Outputs: nothing.
        - deselectKey(key)
            - This takes away one key's highlighting on the keyboard
            - Receives: a char for which key is not to be highlighted
            - Outputs: nothing.
        - changeLetter(direction)
            - This highlights a different letter on the keyboard depending on the direction that a user moves the cursor, and takes away the highlighting from the previously-highlighted letter. It uses selectKey and deselectKey to do this.
            - Receives: a keyboardDirection enum for which direction to move the cursor in
            - Outputs: nothing.
        - getCurrentLetter()
            - Receives: nothing.
            - Outputs: a char, the currently selected letter on the keyboard
        - inputKey(currentWord)
            - This changes the input String depending on which letter is currently selected
            - Receives: a String
            - Outputs: the input String modified depending on which letter is currently selected
    - Private:
        - keyboardHeight
            - An integer that represents the y-level of the keyboard, used when highlighting letters
        - caps
            - A boolean that represents whether or not the keyboard is capitalised, used when printing the keyboard and getting the current letter
        - currentLetter
            - A character, the letter that is currently selected. Initialised to 1 so that the cursor is in the top-right of the keyboard.

## PageHandler.h

This contains all of the handlers for pages in order to make page display more modular and clean up the main file

### Imported includes:

None.

### Self-defined includes:

- AdafruitST7735_Keyboard.h

### Defines

These are used to take Serial monitor input in a more readable way

- LEFT: **a**
- UP: **w**
- RIGHT: **d**
- DOWN: **s**
- SELECT: (space)

### Page enum

This is used to keep track of which page is currently being displayed, has just been displayed, or will be displayed

### String in

This is temporarily used for taking Serial monitor input before soldering buttons in the system.

### Functions

- setCreds(tft, preferences)
    - This function is used to set the credentials of the user (SSID, WiFi password, API ID, API key) in flash memory
    - Receives: an Adafruit_ST7735Keyboard pointer to the display object, a pointer to the Preferences object
    - Outputs: nothing
- MenuPageHandler(tft, previousPage)
    - Handles the main boot-up page
    - Receives: an Adafruit_ST7735Keyboard pointer to the display object, a pointer to the previous Page so that it can update its value
    - Outputs: the next Page to be displayed
- SummaryPageHandler(tft, previousPage, summary)
    - Handles the summary page
    - Receives: an Adafruit_ST7735Keyboard pointer to the display object, a pointer to the previous Page so that it can update its value, and the Summary of the linked Trading212 account
    - Outputs: the next Page to be displayed
- PositionsPageFlipLeft(positions, currentPositions)
    - Moves up the positions linked list until it reaches the top position of the page on the left of the current page
    - Receives: a Positions pointer to the whole positions linked list, and a Positions pointer to the position that is at the top of the current page
    - Outputs: a Positions pointer to the position at the top of the page on the left of the current page
- PositionsPageFlipRight(positions, currentPositions)
    - Moves down the positions linked list until it reaches the top position of the page on the right of the current page
    - Receives: a Positions pointer to the whole positions linked list, and a Positions pointer to the position that is at the top of the current page
    - Outputs: a Positions pointer to the position at the top of the page on the right of the current page
- BuySellPositionHandler(tft, WiFi, position, buy)
    - Handles the buy/sell popup on a detailed position page
    - Receives: an Adafruit_ST7735Keyboard pointer to the display object, a WiFiClass pointer to the WiFi object, the Position to be bought or sold, a boolean that represents whether the user wishes to buy more of this position or sell some quantity
    - Outputs: the next Page to be displayed
- PositionPageHandler(tft, WiFi, currentPositions, positionsSize)
    - Handles the detailed position page
    - Receives: an Adafruit_ST7735Keyboard pointer to the display object, a WiFiClass pointer to the WiFi object, a pointer to the current position in the Positions linked list, the total number of Positions returned by the positions endpoint of the Trading212 API endpoint
    - Outputs: the next Page to be displayed
- PositionsPageHandler(tft, WiFi, previousPage, positions)
    - Handles the positions page
    - Receives: an Adafruit_ST7735Keyboard pointer to the display object, a WiFiClass pointer to the WiFi object, a pointer to the previous Page, a Positions pointer to the positions linked list
    - Outputs: the next Page to be displayed

## main.cpp

### Imported includes

- Adruino.h
- base64.hpp
- Prefernces.h

### Self-defined includes

- PageHandler.h

### Global variables

- Page currentPage
    - This is the page that is currently going to be displayed
- Page previousPage
    - This is the page that was displayed before the current page
- Preferences preferences
    - This is the object that allows for storing in flash memory
- Adafruit_ST7735Keyboard tft
    - This is the object that allows writing to the display
- String apiPair
    - This stores the unencoded Trading212 API ID/key pair of the user
- unsigned char encoded
    - This stores the output of the encoding of the Trading212 API ID/key pair of the user, it must be an unsigned char pointer of constant size because of the way that the encoding function outputs things
- String encodedPair
    - This stores what is in the unsigned char pointer encoded, but rather than being of constant size, it only uses takes the non-empty part of encoded
- Summary summary
    - This is an object that stores what is returned by the Trading212 account summary endpoint
- Positions *positions
    - This is the pointer to the linked list that is made from the return value of the Trading212 open positions endpoint
- cJSON *positionsJson
    - This stores the json response from the Trading212 positions endpoint
- bool positionsExists
    - This is used to keep track of whether or not the Trading212 positions API endpoint has been called as its response takes up a lot of memory so we don't want to make multiple calls to it.

### Setup

- This is only run once, on the boot-up of the chip.
- Initialises the display object and the preferences object.
- Encodes the Trading 212 API credentials
- Intiates a connection with the WiFi network using the credentials in flash memory

### Loop

This runs repeatedly, it calls the correct page handler when it's needed.