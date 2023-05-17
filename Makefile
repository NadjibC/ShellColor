file=src/main.cpp
output=shellcolor
arguments=-t "Hello World!" -b "GREEN" -f "BLACK"

builddir = bin

default: build

all: build run

build:
	@mkdir -p ${builddir}
	@echo -e "\e[31m - Building: ${file}:\e[0m"
	g++ -o ${builddir}/${output} ${file}
run:
	@echo -e "\n\e[31m - Running: ${output}:\e[0m"
	@echo -e "-------------------------------------\n"
	@${builddir}/${output} ${arguments}
	@echo -e ""
