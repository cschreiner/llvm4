

random: random.cpp
	g++ -std=c++11 `llvm-config --cxxflags` -c random.cpp
	g++ `llvm-config --ldflags --system-libs` \
	    random.o \
	    `llvm-config --libs` -lpthread \
	    -o random 

#	g++ -std=c++11 \
#	    `llvm-config --cxxflags --ldflags --libs ` \
#	    random.cpp -o random

#	clang++ `llvm-config --cxxflags` -c random.cpp
#	clang++ `llvm-config --ldflags` \
#	    -stdlib=libc++ \
#	    `llvm-config --libs` -o random random.o
