echo "Compiling,,"
if g++ -c -std=c++11 *.cpp ; then
	echo "Linking.."
	g++ *.o -o SickCityGame -lsfml-graphics -lsfml-window -lsfml-system
	echo "Cleanup.."
fi
echo "Done.";
