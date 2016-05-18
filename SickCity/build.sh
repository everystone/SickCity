echo "Compiling,,"
g++ -c -std=c++11 *.cpp
echo "Linking.."
g++ *.o -o SickCityGame -lsfml-graphics -lsfml-window -lsfml-system
echo "Cleanup.."
echo "Done.";
