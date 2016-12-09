all:	euclideanDis
	
main:
	g++ -Wall -Wvla -Werror -g -D_GLIBCXX_DEBUG -std=c++11 main.cpp -o example
	#./example
	
	#g++ -Wall -g -std=c++11  -c test_Learning.cpp  
	#g++ -Wall -g -std=c++11  test_Learning.o -o test-learning
euclideanDis: euclideanDis.o	
	g++ -Wall -g -pg -std=c++11  euclideanDis.o -o euclideanDis -larmadillo 

euclideanDis.o: euclideanDis.cpp Learning.hpp IDpair.hpp ID.hpp Grid.hpp
	g++  -Wall -g -pg -std=c++11  -c  euclideanDis.cpp
	

