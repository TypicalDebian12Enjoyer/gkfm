SRCS = $(shell find src -name "*.cpp")
OBJS = $(patsubst src/%.cpp, build/obj/%.o, $(SRCS))
DEPS = $(patsubst src/%.cpp, build/dep/%.d, $(SRCS))
TARGET = gkfm # Skrót od "Graficzny Kalkulator Funkcji Matematycznych"
CXX = g++
CXXFLAGS = -std=c++17 `pkg-config --cflags gtkmm-4.0` -MMD -MF $(patsubst build/obj/%.o,build/dep/%.d,$@) -Iinclude
LDFLAGS = `pkg-config --libs gtkmm-4.0`

build/obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(patsubst build/obj/%.o,build/dep/%.d,$@))
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

run: $(TARGET)
	./$(TARGET)

clean-dep:
	rm -rf build/dep

clean-obj:
	rm -rf build/obj

clean-bin:
	rm -f $(TARGET)

clean: clean-dep clean-obj clean-bin

.PHONY: all clean clean-bin clean-obj clean-dep run

-include $(DEPS)
