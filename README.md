# Instructions

  1. Copy the `OLED_Display_128x64` directory to your Arduino libraries directory.
  2. Start the Arduino IDE
  3. Go to File->Preferences. Add
     `http://download.labs.mediatek.com/package_mtk_linkit_smart_7688_test_index.json`
     to Additional Boards Manager URLs
  4. Go to Tools->Board->Board Managers. Search for LinkIt Smart 7688 Duo and
     install
  5. Select Tools->Board->LinkIt Smart 7688 Duo
  6. Plug the LinkIt Smart 7688 Duo in to your PC
  7. Select Tools->Port->(your LinkIt Smart's port)
  8. Load `badge_firmata.ino` into the Arduino IDE
  9. Run Sketch->Upload

# Creating the images

convert ~/nerves/nerves-icon.png -resize 128x40 -threshold 95% -gravity center -extent 128x40 -negate nerves-small.png
~/experiments/png2oled/png2oled nerves-small.png

convert ~/nerves/nerves-icon.png -resize 128x64 -threshold 95% -gravity center -extent 128x64 -negate nerves-big.png
~/experiments/png2oled/png2oled nerves-big.png
