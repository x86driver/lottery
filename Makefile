TARGET = lottery

all:$(TARGET)

lottery:lottery.cpp
	g++ -Wall -o $@ $< -g

clean:
	rm -rf $(TARGET)

