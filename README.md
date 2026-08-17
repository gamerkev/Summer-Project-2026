# Project docs

## jsonGetters.h

This header provides a class called extractData. The project uses the cJSON library to parse the response from APIs but since there are multiple responses and multiple values are needed from them, the cJSON library by itself doesn't give the cleanest code since it uses a linked-list style approach the code would end up having very long lines full of cjsonObject->next->next->child->next.......

This header file solves that with methods that abstract away from those long lines.

### Imported includes:

- cJSON.h

### Self-defined includes:

- None.

### currency enum

This enum has a corresponding value for every currency code that the Trading212 API may return. This makes it easier to print currency symbols to the display since the display is not able to have a glyph for multi-character symbols such as €. For this reason, I'll have to have each currency character represented by a different character in a font file for the display, using numbers for the characters makes this enum very useful!

### Functions:

- currencySymol
    - Returns: currency (enum)
    - Arguments: currCode (String)
    - This function converts the currency code returned by the Trading212 API, which is a String, into the currency enum so that the currency symbol can be easily printed to the display.

### Classes:

- extractData
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
