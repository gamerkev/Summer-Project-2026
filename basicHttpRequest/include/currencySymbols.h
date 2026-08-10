const uint8_t CurrencySymbols_Bitmaps[] PROGMEM = {
  0x64, 0xA1, 0xC8, 0x43, 0xC0, 0x64, 0xA1, 0xC8, 0x49, 0x80, 0x23, 0xE8, 
  0xE2, 0x97, 0xC4
};

const GFXglyph CurrencySymbols_Glyphs[] PROGMEM = {
  {     0,   5,   7,   5,    0,   -7 },   // 0x30 '0'
  {     5,   5,   7,   5,    0,   -7 },   // 0x31 '1'
  {    10,   5,   8,   5,    0,   -7 }    // 0x32 '2'
};

const GFXfont CurrencySymbols PROGMEM = {(uint8_t *) CurrencySymbols_Bitmaps,    (GFXglyph *)CurrencySymbols_Glyphs, 0x30, 0xA3,    7};