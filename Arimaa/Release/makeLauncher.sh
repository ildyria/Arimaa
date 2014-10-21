echo "[Desktop Entry]" >> arimaa-by-gabriel.desktop
echo "Type=Application" >> arimaa-by-gabriel.desktop
echo "Version=1.0" >> arimaa-by-gabriel.desktop
echo "Name=Arimaa" >> arimaa-by-gabriel.desktop
echo "GenericName=Game" >> arimaa-by-gabriel.desktop
echo "Comment=Two player game" >> arimaa-by-gabriel.desktop
echo "Path=$PWD/" >> arimaa-by-gabriel.desktop
echo "Exec=$PWD/Arimaa" >> arimaa-by-gabriel.desktop
echo "Icon=$PWD/Assets/Graphics/Icon_big.png" >> arimaa-by-gabriel.desktop
echo "Terminal=false" >> arimaa-by-gabriel.desktop
echo "Categories=Game" >> arimaa-by-gabriel.desktop
chmod u+x arimaa-by-gabriel.desktop
mv arimaa-by-gabriel.desktop /usr/share/applications/
