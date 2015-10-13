CXX_FLAGS = -lrt -lpthread -g -fpermissive

discar:
	g++ discar.cpp network.cpp algorithm.cpp main.cpp -o discar $(CXX_FLAGS)
	g++ launch.cpp -o launch $(CXX_FLAGS)

all: discar launch

clean:
	rm discar launch -f
