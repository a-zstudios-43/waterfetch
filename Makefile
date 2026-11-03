CXX = g++
CXXFLAGS = -std=c++17 -O2

waterfetch: src/waterfetch.cpp
	$(CXX) $(CXXFLAGS) src/waterfetch.cpp -o waterfetch

clean:
	rm -f waterfetch
install:
	sudo mkdir -p /usr/local/share/waterfetch
	sudo cp logos/*.txt /usr/local/share/waterfetch/
	sudo cp waterfetch /usr/local/bin/