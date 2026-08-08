const uint8_t Pound_Bitmaps[] PROGMEM = {
  0x03, 0x25, 0x0E, 0x42, 0x1E
};

const GFXglyph Pound_Glyphs[] PROGMEM = {
  {     0,   5,   8,   5,    0,   -8 }    // 0xA3 '£'
};

const GFXfont Pound PROGMEM = {(uint8_t *) Pound_Bitmaps,  (GFXglyph *)Pound_Glyphs, 0xA3, 0xA3,  8};