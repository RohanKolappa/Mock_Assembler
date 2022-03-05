//
// Created by Rohan Kolappa on 2/21/22.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <iomanip>
#include <algorithm>

//write an assembler that converts assembly code to machine code.
//Forward references necessitate two-pass assembly. The first pass
//should determine all labels’ values, and the
//second pass should generate the machine code.

std::string twosComplement(std::string x) {
  std::string tempBitString = "";

  int strLength = x.size() - 1;

  int cmdLineInt = stoi(x);

  int absVal = abs(cmdLineInt);

  if (cmdLineInt >= 0) {
    strLength += 1;
  }

  if (cmdLineInt < 0) {

    int complement = ~absVal + 1;
    int rightShift;
    for (int bitNum = 31; bitNum >= 0; bitNum--) {
      rightShift = complement >> bitNum;

      if (rightShift & 1) {
        tempBitString.append("1");
      } else {
        tempBitString.append("0");
      }

    }
  } else {

    int rightShift;
    for (int bitNum = 31; bitNum >= 0; bitNum--) {
      rightShift = cmdLineInt >> bitNum;

      if (rightShift & 1) {
        tempBitString.append("1");
      } else {
        tempBitString.append("0");
      }

    }

  }
  return tempBitString;
}






int main (int argc, char **argv) {

  if (argc == 2) {
    //ofstream var (so we can open file)
    std::ifstream ahrinFile;

    std::string currLine; //temporaily holds the current line
    std::vector<std::string> allLines; //container to hold each string/line
    std::vector<std::string> allLabels;
    std::vector<int> labelValues;

    std::unordered_map<std::string, int> bitMap;

    //may need to change these to work properly
    bitMap["mov"] = 4;
    bitMap["add"] = 4;
    bitMap["cmp"] = 4;
    bitMap["push"] = 4;
    bitMap["pop"] = 4;
    bitMap["call"] = 4;
    bitMap["je"] = 4;
    bitMap["jge"] = 4;
    bitMap["jl"] = 4;
    bitMap["j"] = 4;
    bitMap["ret"] = 4;
    bitMap["nop"] = 4;

    bitMap["$"] = 2;
    bitMap["R1"] = 3;
    bitMap["R2"] = 3;
    bitMap["R3"] = 3;
    bitMap["R4"] = 3;
    bitMap["R5"] = 3;
    bitMap["R6"] = 3;
    bitMap["RS"] = 3;
    bitMap["RB"] = 3;









    int bitCounter = 0;
    int byteCounter = 0;
    size_t stringPosition = 0;
    size_t altStringPosition = 0;
    //get file name from command line
    std::string filename = argv[1];

    //opens file
    ahrinFile.open(filename);

    if (ahrinFile.is_open()) {
      //do actual work on file here

      //pushes each line into vector of strings
      while (getline(ahrinFile, currLine)) {
        allLines.push_back(currLine);
      }

      //do first pass here through allLines vector to create symbol table
      //create a symbol table here because we don't know what each line will be

      for (std::string x : allLines) {
        if (x[0] != ' ') { //label
          allLabels.push_back(x);
          labelValues.push_back(byteCounter); //TODO: check the math on this again

          //std::cout << x << std::endl;
          //std::cout << labelValues.back() << std::endl;

          //create another vector that hold the byte values in same order as the labels appear
        } else if (x[0] == ' ') { //variable or instruction
          if (x.substr(4, 3) == "var") { //variable
            bitCounter+=32; //use bitcounter here
            byteCounter = (bitCounter+7)/8;
            bitCounter = byteCounter * 8;
            //std::cout << byteCounter << std::endl;
          } else if (x.substr(4, 3) == "mov") {
            bitCounter+= 8;
            x = x.substr(8, x.length() - 4);
            while ((stringPosition = x.find(" ")) != std::string::npos) {
              //access unordered_map here and based on the value stored add to bitCounter accordingly
              //std::cout << x << std::endl;
              if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
                altStringPosition = stringPosition;
                stringPosition = x.find(" ");
                if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                  bitCounter += 35; //#(register) encountered
                } else if (x.substr(0, stringPosition).at(0) == '$') {
                  bitCounter += 32; //32 bit number

                } else {
                  bitCounter += 32; //label encountered
                }
              } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
                bitCounter += 32;

              } else {
                bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
              }

              x.erase(0, stringPosition + 1);
              //byteCounter = (bitCounter+7)/8;

              if ((stringPosition = x.find(" ")) == std::string::npos) {
                if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
                  if ((stringPosition = x.find("(")) != std::string::npos) {
                    bitCounter += 35; //#(register) encountered
                  } else if (x.substr(0, stringPosition).at(0) == '$') {
                    bitCounter += 32; //32 bit number

                  } else {
                    bitCounter += 32; //label encountered
                  }
                } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
                  bitCounter += 32;

                } else {
                  bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
                }

              }
            }
            byteCounter = (bitCounter+7)/8;
            bitCounter = byteCounter * 8;

          } else if (x.substr(4, 3) == "cmp") {
            bitCounter+= 8;
            x = x.substr(8, x.length() - 4);
            while ((stringPosition = x.find(" ")) != std::string::npos) {
              //access unordered_map here and based on the value stored add to bitCounter accordingly
              //std::cout << x << std::endl;
              if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
                altStringPosition = stringPosition;
                stringPosition = x.find(" ");
                if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                  bitCounter += 35; //#(register) encountered
                } else if (x.substr(0, stringPosition).at(0) == '$') {
                  bitCounter += 32; //32 bit number

                } else {
                  bitCounter += 32; //label encountered
                }
              } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
                bitCounter += 32;

              } else {
                bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
              }

              x.erase(0, stringPosition + 1);
              //byteCounter = (bitCounter+7)/8;

              if ((stringPosition = x.find(" ")) == std::string::npos) {
                if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
                  if ((stringPosition = x.find("(")) != std::string::npos) {
                    bitCounter += 35; //#(register) encountered
                  } else if (x.substr(0, stringPosition).at(0) == '$') {
                    bitCounter += 32; //32 bit number

                  } else {
                    bitCounter += 32; //label encountered
                  }
                } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
                  bitCounter += 32;

                } else {
                  bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
                }

              }
            }
            byteCounter = (bitCounter+7)/8;
            bitCounter = byteCounter * 8;

          } else if (x.substr(4, 3) == "add") {

            bitCounter+= 8;
            x = x.substr(8, x.length() - 4);
            while ((stringPosition = x.find(" ")) != std::string::npos) {
              //access unordered_map here and based on the value stored add to bitCounter accordingly
              //std::cout << x << std::endl;
              if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
                altStringPosition = stringPosition;
                stringPosition = x.find(" ");
                if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                  bitCounter += 35; //#(register) encountered
                } else if (x.substr(0, stringPosition).at(0) == '$') {
                  bitCounter += 32; //32 bit number

                } else {
                  bitCounter += 32; //label encountered
                }
              } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
                bitCounter += 32;

              } else {
                bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
              }

              x.erase(0, stringPosition + 1);
              //byteCounter = (bitCounter+7)/8;

              if ((stringPosition = x.find(" ")) == std::string::npos) {
                if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
                  if ((stringPosition = x.find("(")) != std::string::npos) {
                    bitCounter += 35; //#(register) encountered
                  } else if (x.substr(0, stringPosition).at(0) == '$') {
                    bitCounter += 32; //32 bit number

                  } else {
                    bitCounter += 32; //label encountered
                  }
                } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
                  bitCounter += 32;

                } else {
                  bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
                }

              }
            }
            byteCounter = (bitCounter+7)/8;
            bitCounter = byteCounter * 8;

          } else if (x.substr(4, 3) == "cmp") {
            bitCounter+= 8;
            x = x.substr(8, x.length() - 4);
            while ((stringPosition = x.find(" ")) != std::string::npos) {
              //access unordered_map here and based on the value stored add to bitCounter accordingly
              //std::cout << x << std::endl;
              if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
                altStringPosition = stringPosition;
                stringPosition = x.find(" ");
                if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                  bitCounter += 35; //#(register) encountered
                } else if (x.substr(0, stringPosition).at(0) == '$') {
                  bitCounter += 32; //32 bit number

                } else {
                  bitCounter += 32; //label encountered
                }
              } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
                bitCounter += 32;

              } else {
                bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
              }

              x.erase(0, stringPosition + 1);
              //byteCounter = (bitCounter+7)/8;

              if ((stringPosition = x.find(" ")) == std::string::npos) {
                if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
                  if ((stringPosition = x.find("(")) != std::string::npos) {
                    bitCounter += 35; //#(register) encountered
                  } else if (x.substr(0, stringPosition).at(0) == '$') {
                    bitCounter += 32; //32 bit number

                  } else {
                    bitCounter += 32; //label encountered
                  }
                } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
                  bitCounter += 32;

                } else {
                  bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
                }

              }
            }
            byteCounter = (bitCounter+7)/8;
            bitCounter = byteCounter * 8;

          } else if (x.substr(4, 3) == "cmp") {
            bitCounter+= 8;
            x = x.substr(8, x.length() - 4);
            while ((stringPosition = x.find(" ")) != std::string::npos) {
              //access unordered_map here and based on the value stored add to bitCounter accordingly
              //std::cout << x << std::endl;
              if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
                altStringPosition = stringPosition;
                stringPosition = x.find(" ");
                if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                  bitCounter += 35; //#(register) encountered
                } else if (x.substr(0, stringPosition).at(0) == '$') {
                  bitCounter += 32; //32 bit number

                } else {
                  bitCounter += 32; //label encountered
                }
              } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
                bitCounter += 32;

              } else {
                bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
              }

              x.erase(0, stringPosition + 1);
              //byteCounter = (bitCounter+7)/8;

              if ((stringPosition = x.find(" ")) == std::string::npos) {
                if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
                  if ((stringPosition = x.find("(")) != std::string::npos) {
                    bitCounter += 35; //#(register) encountered
                  } else if (x.substr(0, stringPosition).at(0) == '$') {
                    bitCounter += 32; //32 bit number

                  } else {
                    bitCounter += 32; //label encountered
                  }
                } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
                  bitCounter += 32;

                } else {
                  bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
                }

              }
            }
            byteCounter = (bitCounter+7)/8;
            bitCounter = byteCounter * 8;

          } else if (x.substr(4, 3) == "pop") {
            bitCounter+= 6;
            x = x.substr(8, x.length() - 4);
            if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
              altStringPosition = stringPosition;
              stringPosition = x.find(" ");
              if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                bitCounter += 35; //#(register) encountered
              } else if (x.substr(0, stringPosition).at(0) == '$') {
                bitCounter += 32; //32 bit number
                //11001000
              } else {
                bitCounter += 32; //label encountered
              }
            } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
              bitCounter += 32;

            } else {
              bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
            }



            byteCounter = (bitCounter+7)/8;
            bitCounter = byteCounter * 8;



          } else if (x.substr(4, 4) == "call") {
            bitCounter+= 6;
            x = x.substr(9, x.length() - 4);
            if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
              altStringPosition = stringPosition;
              stringPosition = x.find(" ");
              if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                bitCounter += 35; //#(register) encountered
              } else if (x.substr(0, stringPosition).at(0) == '$') {
                bitCounter += 32; //32 bit number

              } else {
                bitCounter += 32; //label encountered
              }
            } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
              bitCounter += 32;

            } else {
              bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
            }



            byteCounter = (bitCounter+7)/8;
            bitCounter = byteCounter * 8;


          } else if (x.substr(4, 2) == "je") {
            bitCounter+= 6;
            x = x.substr(7, x.length() - 4);
            if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
              altStringPosition = stringPosition;
              stringPosition = x.find(" ");
              if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                bitCounter += 35; //#(register) encountered
              } else if (x.substr(0, stringPosition).at(0) == '$') {
                bitCounter += 32; //32 bit number

              } else {
                bitCounter += 32; //label encountered
              }
            } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
              bitCounter += 32;

            } else {
              bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
            }



            byteCounter = (bitCounter+7)/8;
            bitCounter = byteCounter * 8;



          } else if (x.substr(4, 3) == "jge") {
            bitCounter+= 6;
            x = x.substr(8, x.length() - 4);
            if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
              altStringPosition = stringPosition;
              stringPosition = x.find(" ");
              if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                bitCounter += 35; //#(register) encountered
              } else if (x.substr(0, stringPosition).at(0) == '$') {
                bitCounter += 32; //32 bit number

              } else {
                bitCounter += 32; //label encountered
              }
            } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
              bitCounter += 32;

            } else {
              bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
            }



            byteCounter = (bitCounter+7)/8;
            bitCounter = byteCounter * 8;



          } else if (x.substr(4, 2) == "jl") {
            bitCounter+= 6;
            x = x.substr(7, x.length() - 4);
            if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
              altStringPosition = stringPosition;
              stringPosition = x.find(" ");
              if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                bitCounter += 35; //#(register) encountered
              } else if (x.substr(0, stringPosition).at(0) == '$') {
                bitCounter += 32; //32 bit number

              } else {
                bitCounter += 32; //label encountered
              }
            } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
              bitCounter += 32;

            } else {
              bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
            }



            byteCounter = (bitCounter+7)/8;
            bitCounter = byteCounter * 8;


          } else if (x.substr(4, 1) == "j") {
            bitCounter+= 6;
            x = x.substr(6, x.length() - 4);
            if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) {
              altStringPosition = stringPosition;
              stringPosition = x.find(" ");
              if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                bitCounter += 35; //#(register) encountered
              } else if (x.substr(0, stringPosition).at(0) == '$') {
                bitCounter += 32; //32 bit number

              } else {
                bitCounter += 32; //label encountered
              }
            } else if (x.substr(0, stringPosition).at(0) == '$') { //direct addressing
              bitCounter += 32;

            } else {
              bitCounter += bitMap.at(x.substr(0, stringPosition)); //register
            }



            byteCounter = (bitCounter+7)/8;
            bitCounter = byteCounter * 8;



          } else if (x.substr(4, 3) == "ret") {
            bitCounter+= 4;
            byteCounter = (bitCounter+7)/8;
            bitCounter = byteCounter * 8;

          } else if (x.substr(4, 3) == "nop") {
            bitCounter+= 4;
            byteCounter = (bitCounter+7)/8;
            bitCounter = byteCounter * 8;

          }




          //create a dictionary/map here
          //key = string that represents an instruction
          //value = number of bits of that command
          //parse the string/line until i hit whitespace or end of line
          //then i have to search the map for the key
          //then add the appropriate numbers of bits (determined by value in map)
          // to the bitCounter int variable
          //also make sure to pad the bits to the nearest number of bytes


        }
      }

//      for (int x : labelValues) {
//          std::cout << x << std::endl;
//      }



      std::vector<std::string> sortedAllLabels = allLabels;
      sort(sortedAllLabels.begin(), sortedAllLabels.end());


      std::string tempLabel;
      for (unsigned int i = 1; i < sortedAllLabels.size(); i++) {
        tempLabel = sortedAllLabels[i - 1];
        if (tempLabel == sortedAllLabels[i]) {
          std::cerr << "At least one duplicate label found.\n";
          exit(0);
        }
      }




      //TODO: PART 2 --> do the same thing as before except have to append to string vector
      //TODO: for each line and have to determine what addressing mode I'm using for opcode

      //info i have now:
      //vector of all labels
      //parallel vector with label values
      //vector with each element being a string with one line of code

      //what i need to do:
      //figure out how to calculate two's complement for negative numbers
      //figure out how to separate numbers (including negatives) from whitespace and other symbols
      //maybe use bitwise operators to convert empty 0 bits to proper values (create space with
      //make one long string with 0's for each part of one instruction then before appending to long string use bitwise operators

      std::unordered_map<std::string, std::string> opcodeMap;
      std::string tempBitString = "";
      std::string int32bit = "";
      std::string opcode4bit = "";

      std::vector<std::string> eachOutputLine; //the byte #: part can be prepended

      for (unsigned int i = 0; i < allLabels.size(); i++) {
        opcodeMap[allLabels[i]] = std::to_string(labelValues[i]);
        // std::cout << opcodeMap[allLabels[i]] << std::endl;
      } //now i have a map for the labels and can convert the numbers from string to int to two's complement and then append

      //convert label values in vector into 2's complement? --> prolly just base 2 since they're positive
      //use code from the earlier programs for this

      //may need to change these to work properly
      opcodeMap["mov"] = "0000";
      opcodeMap["add"] = "0001";
      opcodeMap["cmp"] = "0010";
      opcodeMap["push"] = "0011";
      opcodeMap["pop"] = "0100";
      opcodeMap["call"] = "0101";
      opcodeMap["je"] = "0110";
      opcodeMap["jge"] = "0111";
      opcodeMap["jl"] = "1000";
      opcodeMap["j"] = "1001";
      opcodeMap["ret"] = "1010";
      opcodeMap["nop"] = "1011";

      opcodeMap["$"] = "00";
      opcodeMap["R1"] = "000";
      opcodeMap["R2"] = "001";
      opcodeMap["R3"] = "010";
      opcodeMap["R4"] = "011";
      opcodeMap["R5"] = "100";
      opcodeMap["R6"] = "101";
      opcodeMap["RS"] = "110";
      opcodeMap["RB"] = "111";

//TODO: ---------------------------------------------------------------------------------

      for (std::string x : allLines) {
        if (x[0] != ' ') { //label
          //do nothing pretty much

          //create another vector that hold the byte values in same order as the labels appear
        } else if (x[0] == ' ') { //variable or instruction
          if (x.substr(4, 3) == "var") { //variable
            x = x.substr(8, x.length() - 4);
            int32bit = "";
            int32bit.append(twosComplement(x));
            //std::cout << int32bit << std::endl;
            tempBitString.append(int32bit);

          } else if (x.substr(4, 3) == "mov") {
            tempBitString.append(opcodeMap[x.substr(4, 3)]);
            x = x.substr(8, x.length() - 4);
            while ((stringPosition = x.find(" ")) != std::string::npos) {
              //access unordered_map here and based on the value stored add to bitCounter accordingly
              //std::cout << x << std::endl;
              if (opcodeMap.find(x.substr(0, stringPosition)) == opcodeMap.end()) { //enter if not found in map
                altStringPosition = stringPosition;
                stringPosition = x.find(" ");
                //std::cout << x.substr(0, stringPosition) << std::endl;
                if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                  //indexed mode
                  tempBitString.append("11"); //#(register) encountered
                  //all space between whitespace and "(" (to isolate number)

                  std::string z = x.substr(0, stringPosition);
                  //int z_orig_length = x.substr(0, stringPosition).length();
                  z.pop_back();
                  z.pop_back();
                  z.pop_back();
                  z.pop_back();

                  tempBitString.append(twosComplement(z));
                  std::string y = x.substr(0, stringPosition);
                  y.erase(0, z.length() + 1);
                  y.pop_back();
                  tempBitString.append(opcodeMap[y]);
                  //x.erase(0, z_orig_length + 1);
//                  if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
//                    z = x.substr(0, stringPosition);
//                    z_orig_length = x.substr(0, stringPosition).length();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//
//                    tempBitString.append(twosComplement(z));
//                    y = x.substr(0, stringPosition);
//                    y.erase(0, z.length() + 1);
//                    y.pop_back();
//                    tempBitString.append(opcodeMap[y]);
//
//                  } //else {
//                    x.erase(0, z.length() + 1);
//                    x.pop_back();
//                    tempBitString.append(opcodeMap[x]);
//                  }

                } else if (x.substr(0, stringPosition).at(0) == '$') {
                  //immediate mode
                  tempBitString.append("00");
                  std::string y = x.substr(1, x.length());
                  if (opcodeMap.find(y) == opcodeMap.end()) { //can be label or number
                    tempBitString.append(twosComplement(y)); //number

                  } else {
                    tempBitString.append(twosComplement(opcodeMap[y]));
                  }

                }
              } else if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) { //if it's not in bitmap then it's a label, if it is then a register
                //direct mode
                tempBitString.append("10");
                tempBitString.append(twosComplement(opcodeMap[x.substr(0, stringPosition)])); //append label value

              } else {
                //register mode
                tempBitString.append("01");
                tempBitString.append(opcodeMap[x.substr(0, stringPosition)]); //append register value

              }


              x.erase(0, stringPosition+1);

              //byteCounter = (bitCounter+7)/8;

              if ((stringPosition = x.find(" ")) == std::string::npos) {
                if (opcodeMap.find(x.substr(0, stringPosition)) == opcodeMap.end()) { //enter if not found in map
                  altStringPosition = stringPosition;
                  stringPosition = x.find(" ");
                  if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                    //indexed mode
                    tempBitString.append("11"); //#(register) encountered
                    //all space between whitespace and "(" (to isolate number)
                    std::string z = x;
                    z.pop_back();
                    z.pop_back();
                    z.pop_back();
                    z.pop_back();

                    tempBitString.append(twosComplement(z));
                    x.erase(0, z.length() + 1);
                    x.pop_back();
                    tempBitString.append(opcodeMap[x]);

                  } else if (x.substr(0, stringPosition).at(0) == '$') {
                    //immediate mode
                    tempBitString.append("00");
                    std::string y = x.substr(1, x.length());
                    if (opcodeMap.find(y) == opcodeMap.end()) { //can be label or number
                      tempBitString.append(twosComplement(y)); //number

                    } else {
                      tempBitString.append(twosComplement(opcodeMap[y]));
                    }

                  }
                } else if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) { //if it's not in bitmap then it's a label, if it is then a register
                  //direct mode
                  tempBitString.append("10");
                  tempBitString.append(twosComplement(opcodeMap[x.substr(0, stringPosition)])); //append label value

                } else {
                  //register mode
                  tempBitString.append("01");
                  tempBitString.append(opcodeMap[x.substr(0, stringPosition)]); //append register value

                }


              }
            }

          } else if (x.substr(4, 3) == "add") {
            tempBitString.append(opcodeMap[x.substr(4, 3)]);
            x = x.substr(8, x.length() - 4);

            while ((stringPosition = x.find(" ")) != std::string::npos) {
              //access unordered_map here and based on the value stored add to bitCounter accordingly
              //std::cout << x << std::endl;
              if (opcodeMap.find(x.substr(0, stringPosition)) == opcodeMap.end()) { //enter if not found in map
                altStringPosition = stringPosition;
                stringPosition = x.find(" ");
               // std::cout << x.substr(0, stringPosition) << std::endl;
                if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                  //indexed mode
                  tempBitString.append("11"); //#(register) encountered
                  //all space between whitespace and "(" (to isolate number)

                  std::string z = x.substr(0, stringPosition);
                  // int z_orig_length = x.substr(0, stringPosition).length();
                  z.pop_back();
                  z.pop_back();
                  z.pop_back();
                  z.pop_back();

                  tempBitString.append(twosComplement(z));
                  std::string y = x.substr(0, stringPosition);
                  y.erase(0, z.length() + 1);
                  y.pop_back();
                  tempBitString.append(opcodeMap[y]);
                  //x.erase(0, z_orig_length + 1);
//                  if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
//                    z = x.substr(0, stringPosition);
//                    z_orig_length = x.substr(0, stringPosition).length();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//
//                    tempBitString.append(twosComplement(z));
//                    y = x.substr(0, stringPosition);
//                    y.erase(0, z.length() + 1);
//                    y.pop_back();
//                    tempBitString.append(opcodeMap[y]);
//
//                  } //else {
//                    x.erase(0, z.length() + 1);
//                    x.pop_back();
//                    tempBitString.append(opcodeMap[x]);
//                  }

                } else if (x.substr(0, stringPosition).at(0) == '$') {
                  //immediate mode
                  tempBitString.append("00");
                  std::string y = x.substr(1, x.length());
                  if (opcodeMap.find(y) == opcodeMap.end()) { //can be label or number
                    tempBitString.append(twosComplement(y)); //number

                  } else {
                    tempBitString.append(twosComplement(opcodeMap[y]));
                  }

                }
              } else if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) { //if it's not in bitmap then it's a label, if it is then a register
                //direct mode
                tempBitString.append("10");
                tempBitString.append(twosComplement(opcodeMap[x.substr(0, stringPosition)])); //append label value

              } else {
                //register mode
                tempBitString.append("01");
                tempBitString.append(opcodeMap[x.substr(0, stringPosition)]); //append register value

              }


              x.erase(0, stringPosition+1);

              //byteCounter = (bitCounter+7)/8;

              if ((stringPosition = x.find(" ")) == std::string::npos) {
                if (opcodeMap.find(x.substr(0, stringPosition)) == opcodeMap.end()) { //enter if not found in map
                  altStringPosition = stringPosition;
                  stringPosition = x.find(" ");
                  if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                    //indexed mode
                    tempBitString.append("11"); //#(register) encountered
                    //all space between whitespace and "(" (to isolate number)
                    std::string z = x;
                    z.pop_back();
                    z.pop_back();
                    z.pop_back();
                    z.pop_back();

                    tempBitString.append(twosComplement(z));
                    x.erase(0, z.length() + 1);
                    x.pop_back();
                    tempBitString.append(opcodeMap[x]);

                  } else if (x.substr(0, stringPosition).at(0) == '$') {
                    //immediate mode
                    tempBitString.append("00");
                    std::string y = x.substr(1, x.length());
                    if (opcodeMap.find(y) == opcodeMap.end()) { //can be label or number
                      tempBitString.append(twosComplement(y)); //number

                    } else {
                      tempBitString.append(twosComplement(opcodeMap[y]));
                    }

                  }
                } else if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) { //if it's not in bitmap then it's a label, if it is then a register
                  //direct mode
                  tempBitString.append("10");
                  tempBitString.append(twosComplement(opcodeMap[x.substr(0, stringPosition)])); //append label value

                } else {
                  //register mode
                  tempBitString.append("01");
                  tempBitString.append(opcodeMap[x.substr(0, stringPosition)]); //append register value

                }

              }
            }


          } else if (x.substr(4, 3) == "cmp") {
            tempBitString.append(opcodeMap[x.substr(4, 3)]);
            x = x.substr(8, x.length() - 4);
            while ((stringPosition = x.find(" ")) != std::string::npos) {
              //access unordered_map here and based on the value stored add to bitCounter accordingly
              //std::cout << x << std::endl;
              if (opcodeMap.find(x.substr(0, stringPosition)) == opcodeMap.end()) { //enter if not found in map
                altStringPosition = stringPosition;
                stringPosition = x.find(" ");
               // std::cout << x.substr(0, stringPosition) << std::endl;
                if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                  //indexed mode
                  tempBitString.append("11"); //#(register) encountered
                  //all space between whitespace and "(" (to isolate number)

                  std::string z = x.substr(0, stringPosition);
                  // int z_orig_length = x.substr(0, stringPosition).length();
                  z.pop_back();
                  z.pop_back();
                  z.pop_back();
                  z.pop_back();

                  tempBitString.append(twosComplement(z));
                  std::string y = x.substr(0, stringPosition);
                  y.erase(0, z.length() + 1);
                  y.pop_back();
                  tempBitString.append(opcodeMap[y]);
                  //x.erase(0, z_orig_length + 1);
//                  if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
//                    z = x.substr(0, stringPosition);
//                    z_orig_length = x.substr(0, stringPosition).length();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//
//                    tempBitString.append(twosComplement(z));
//                    y = x.substr(0, stringPosition);
//                    y.erase(0, z.length() + 1);
//                    y.pop_back();
//                    tempBitString.append(opcodeMap[y]);
//
//                  } //else {
//                    x.erase(0, z.length() + 1);
//                    x.pop_back();
//                    tempBitString.append(opcodeMap[x]);
//                  }

                } else if (x.substr(0, stringPosition).at(0) == '$') {
                  //immediate mode
                  tempBitString.append("00");
                  std::string y = x.substr(1, x.length());
                  if (opcodeMap.find(y) == opcodeMap.end()) { //can be label or number
                    tempBitString.append(twosComplement(y)); //number

                  } else {
                    tempBitString.append(twosComplement(opcodeMap[y]));
                  }

                }
              } else if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) { //if it's not in bitmap then it's a label, if it is then a register
                //direct mode
                tempBitString.append("10");
                tempBitString.append(twosComplement(opcodeMap[x.substr(0, stringPosition)])); //append label value

              } else {
                //register mode
                tempBitString.append("01");
                tempBitString.append(opcodeMap[x.substr(0, stringPosition)]); //append register value

              }


              x.erase(0, stringPosition+1);

              //byteCounter = (bitCounter+7)/8;

              if ((stringPosition = x.find(" ")) == std::string::npos) {
                if (opcodeMap.find(x.substr(0, stringPosition)) == opcodeMap.end()) { //enter if not found in map
                  altStringPosition = stringPosition;
                  stringPosition = x.find(" ");
                  if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                    //indexed mode
                    tempBitString.append("11"); //#(register) encountered
                    //all space between whitespace and "(" (to isolate number)
                    std::string z = x;
                    z.pop_back();
                    z.pop_back();
                    z.pop_back();
                    z.pop_back();

                    tempBitString.append(twosComplement(z));
                    x.erase(0, z.length() + 1);
                    x.pop_back();
                    tempBitString.append(opcodeMap[x]);

                  } else if (x.substr(0, stringPosition).at(0) == '$') {
                    //immediate mode
                    tempBitString.append("00");
                    std::string y = x.substr(1, x.length());
                    if (opcodeMap.find(y) == opcodeMap.end()) { //can be label or number
                      tempBitString.append(twosComplement(y)); //number

                    } else {
                      tempBitString.append(twosComplement(opcodeMap[y]));
                    }

                  }
                } else if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) { //if it's not in bitmap then it's a label, if it is then a register
                  //direct mode
                  tempBitString.append("10");
                  tempBitString.append(twosComplement(opcodeMap[x.substr(0, stringPosition)])); //append label value

                } else {
                  //register mode
                  tempBitString.append("01");
                  tempBitString.append(opcodeMap[x.substr(0, stringPosition)]); //append register value

                }

              }
            }


          } else if (x.substr(4, 4) == "push") {
            tempBitString.append(opcodeMap[x.substr(4, 4)]);
            x = x.substr(9, x.length() - 4);

            //std::cout << x << std::endl;
            if (opcodeMap.find(x.substr(0, stringPosition)) == opcodeMap.end()) { //enter if not found in map
              altStringPosition = stringPosition;
              stringPosition = x.find(" ");
             // std::cout << x.substr(0, stringPosition) << std::endl;
              if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                //indexed mode
                tempBitString.append("11"); //#(register) encountered
                //all space between whitespace and "(" (to isolate number)

                std::string z = x.substr(0, stringPosition);
                // int z_orig_length = x.substr(0, stringPosition).length();
                z.pop_back();
                z.pop_back();
                z.pop_back();
                z.pop_back();

                tempBitString.append(twosComplement(z));
                std::string y = x.substr(0, stringPosition);
                y.erase(0, z.length() + 1);
                y.pop_back();
                tempBitString.append(opcodeMap[y]);
                //x.erase(0, z_orig_length + 1);
//                  if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
//                    z = x.substr(0, stringPosition);
//                    z_orig_length = x.substr(0, stringPosition).length();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//
//                    tempBitString.append(twosComplement(z));
//                    y = x.substr(0, stringPosition);
//                    y.erase(0, z.length() + 1);
//                    y.pop_back();
//                    tempBitString.append(opcodeMap[y]);
//
//                  } //else {
//                    x.erase(0, z.length() + 1);
//                    x.pop_back();
//                    tempBitString.append(opcodeMap[x]);
//                  }

              } else if (x.substr(0, stringPosition).at(0) == '$') {
                //immediate mode
                tempBitString.append("00");
                std::string y = x.substr(1, x.length());
                if (opcodeMap.find(y) == opcodeMap.end()) { //can be label or number
                  tempBitString.append(twosComplement(y)); //number

                } else {
                  tempBitString.append(twosComplement(opcodeMap[y]));
                }

              }
            } else if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) { //if it's not in bitmap then it's a label, if it is then a register
              //direct mode
              tempBitString.append("10");
              tempBitString.append(twosComplement(opcodeMap[x.substr(0, stringPosition)])); //append label value

            } else {
              //register mode
              tempBitString.append("01");
              tempBitString.append(opcodeMap[x.substr(0, stringPosition)]); //append register value

            }


            x.erase(0, stringPosition+1);




          } else if (x.substr(4, 3) == "pop") {
            tempBitString.append(opcodeMap[x.substr(4, 3)]);
            x = x.substr(8, x.length() - 4);

            //std::cout << x << std::endl;
            if (opcodeMap.find(x.substr(0, stringPosition)) == opcodeMap.end()) { //enter if not found in map
              altStringPosition = stringPosition;
              stringPosition = x.find(" ");
             // std::cout << x.substr(0, stringPosition) << std::endl;
              if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                //indexed mode
                tempBitString.append("11"); //#(register) encountered
                //all space between whitespace and "(" (to isolate number)

                std::string z = x.substr(0, stringPosition);
                // int z_orig_length = x.substr(0, stringPosition).length();
                z.pop_back();
                z.pop_back();
                z.pop_back();
                z.pop_back();

                tempBitString.append(twosComplement(z));
                std::string y = x.substr(0, stringPosition);
                y.erase(0, z.length() + 1);
                y.pop_back();
                tempBitString.append(opcodeMap[y]);
                //x.erase(0, z_orig_length + 1);
//                  if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
//                    z = x.substr(0, stringPosition);
//                    z_orig_length = x.substr(0, stringPosition).length();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//
//                    tempBitString.append(twosComplement(z));
//                    y = x.substr(0, stringPosition);
//                    y.erase(0, z.length() + 1);
//                    y.pop_back();
//                    tempBitString.append(opcodeMap[y]);
//
//                  } //else {
//                    x.erase(0, z.length() + 1);
//                    x.pop_back();
//                    tempBitString.append(opcodeMap[x]);
//                  }

              } else if (x.substr(0, stringPosition).at(0) == '$') {
                //immediate mode
                tempBitString.append("00");
                std::string y = x.substr(1, x.length());
                if (opcodeMap.find(y) == opcodeMap.end()) { //can be label or number
                  tempBitString.append(twosComplement(y)); //number

                } else {
                  tempBitString.append(twosComplement(opcodeMap[y]));
                }

              }
            } else if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) { //if it's not in bitmap then it's a label, if it is then a register
              //direct mode
              tempBitString.append("10");
              tempBitString.append(twosComplement(opcodeMap[x.substr(0, stringPosition)])); //append label value

            } else {
              //register mode
              tempBitString.append("01");
              tempBitString.append(opcodeMap[x.substr(0, stringPosition)]); //append register value

            }


            x.erase(0, stringPosition+1);


          } else if (x.substr(4, 4) == "call") {
            tempBitString.append(opcodeMap[x.substr(4, 4)]);
            x = x.substr(9, x.length() - 4);

            //std::cout << x << std::endl;
            if (opcodeMap.find(x.substr(0, stringPosition)) == opcodeMap.end()) { //enter if not found in map
              altStringPosition = stringPosition;
              stringPosition = x.find(" ");
             // std::cout << x.substr(0, stringPosition) << std::endl;
              if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                //indexed mode
                tempBitString.append("11"); //#(register) encountered
                //all space between whitespace and "(" (to isolate number)

                std::string z = x.substr(0, stringPosition);
                //int z_orig_length = x.substr(0, stringPosition).length();
                z.pop_back();
                z.pop_back();
                z.pop_back();
                z.pop_back();

                tempBitString.append(twosComplement(z));
                std::string y = x.substr(0, stringPosition);
                y.erase(0, z.length() + 1);
                y.pop_back();
                tempBitString.append(opcodeMap[y]);
                //x.erase(0, z_orig_length + 1);
//                  if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
//                    z = x.substr(0, stringPosition);
//                    z_orig_length = x.substr(0, stringPosition).length();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//
//                    tempBitString.append(twosComplement(z));
//                    y = x.substr(0, stringPosition);
//                    y.erase(0, z.length() + 1);
//                    y.pop_back();
//                    tempBitString.append(opcodeMap[y]);
//
//                  } //else {
//                    x.erase(0, z.length() + 1);
//                    x.pop_back();
//                    tempBitString.append(opcodeMap[x]);
//                  }

              } else if (x.substr(0, stringPosition).at(0) == '$') {
                //immediate mode
                tempBitString.append("00");
                std::string y = x.substr(1, x.length());
                if (opcodeMap.find(y) == opcodeMap.end()) { //can be label or number
                  tempBitString.append(twosComplement(y)); //number

                } else {
                  tempBitString.append(twosComplement(opcodeMap[y]));
                }

              }
            } else if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) { //if it's not in bitmap then it's a label, if it is then a register
              //direct mode
              tempBitString.append("10");
              tempBitString.append(twosComplement(opcodeMap[x.substr(0, stringPosition)])); //append label value

            } else {
              //register mode
              tempBitString.append("01");
              tempBitString.append(opcodeMap[x.substr(0, stringPosition)]); //append register value

            }


            x.erase(0, stringPosition+1);


          } else if (x.substr(4, 2) == "je") {
            tempBitString.append(opcodeMap[x.substr(4, 2)]);
            x = x.substr(7, x.length() - 4);
            //std::cout << x << std::endl;
            if (opcodeMap.find(x.substr(0, stringPosition)) == opcodeMap.end()) { //enter if not found in map
              altStringPosition = stringPosition;
              stringPosition = x.find(" ");
              //std::cout << x.substr(0, stringPosition) << std::endl;
              if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                //indexed mode
                tempBitString.append("11"); //#(register) encountered
                //all space between whitespace and "(" (to isolate number)

                std::string z = x.substr(0, stringPosition);
                //int z_orig_length = x.substr(0, stringPosition).length();
                z.pop_back();
                z.pop_back();
                z.pop_back();
                z.pop_back();

                tempBitString.append(twosComplement(z));
                std::string y = x.substr(0, stringPosition);
                y.erase(0, z.length() + 1);
                y.pop_back();
                tempBitString.append(opcodeMap[y]);
                //x.erase(0, z_orig_length + 1);
//                  if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
//                    z = x.substr(0, stringPosition);
//                    z_orig_length = x.substr(0, stringPosition).length();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//
//                    tempBitString.append(twosComplement(z));
//                    y = x.substr(0, stringPosition);
//                    y.erase(0, z.length() + 1);
//                    y.pop_back();
//                    tempBitString.append(opcodeMap[y]);
//
//                  } //else {
//                    x.erase(0, z.length() + 1);
//                    x.pop_back();
//                    tempBitString.append(opcodeMap[x]);
//                  }

              } else if (x.substr(0, stringPosition).at(0) == '$') {
                //immediate mode
                tempBitString.append("00");
                std::string y = x.substr(1, x.length());
                if (opcodeMap.find(y) == opcodeMap.end()) { //can be label or number
                  tempBitString.append(twosComplement(y)); //number

                } else {
                  tempBitString.append(twosComplement(opcodeMap[y]));
                }

              }
            } else if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) { //if it's not in bitmap then it's a label, if it is then a register
              //direct mode
              tempBitString.append("10");
              tempBitString.append(twosComplement(opcodeMap[x.substr(0, stringPosition)])); //append label value

            } else {
              //register mode
              tempBitString.append("01");
              tempBitString.append(opcodeMap[x.substr(0, stringPosition)]); //append register value

            }


            x.erase(0, stringPosition+1);



          } else if (x.substr(4, 3) == "jge") {
            tempBitString.append(opcodeMap[x.substr(4, 3)]);
            x = x.substr(8, x.length() - 4);

            //std::cout << x << std::endl;
            if (opcodeMap.find(x.substr(0, stringPosition)) == opcodeMap.end()) { //enter if not found in map
              altStringPosition = stringPosition;
              stringPosition = x.find(" ");
              //std::cout << x.substr(0, stringPosition) << std::endl;
              if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                //indexed mode
                tempBitString.append("11"); //#(register) encountered
                //all space between whitespace and "(" (to isolate number)

                std::string z = x.substr(0, stringPosition);
                //int z_orig_length = x.substr(0, stringPosition).length();
                z.pop_back();
                z.pop_back();
                z.pop_back();
                z.pop_back();

                tempBitString.append(twosComplement(z));
                std::string y = x.substr(0, stringPosition);
                y.erase(0, z.length() + 1);
                y.pop_back();
                tempBitString.append(opcodeMap[y]);
                //x.erase(0, z_orig_length + 1);
//                  if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
//                    z = x.substr(0, stringPosition);
//                    z_orig_length = x.substr(0, stringPosition).length();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//
//                    tempBitString.append(twosComplement(z));
//                    y = x.substr(0, stringPosition);
//                    y.erase(0, z.length() + 1);
//                    y.pop_back();
//                    tempBitString.append(opcodeMap[y]);
//
//                  } //else {
//                    x.erase(0, z.length() + 1);
//                    x.pop_back();
//                    tempBitString.append(opcodeMap[x]);
//                  }

              } else if (x.substr(0, stringPosition).at(0) == '$') {
                //immediate mode
                tempBitString.append("00");
                std::string y = x.substr(1, x.length());
                if (opcodeMap.find(y) == opcodeMap.end()) { //can be label or number
                  tempBitString.append(twosComplement(y)); //number

                } else {
                  tempBitString.append(twosComplement(opcodeMap[y]));
                }

              }
            } else if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) { //if it's not in bitmap then it's a label, if it is then a register
              //direct mode
              tempBitString.append("10");
              tempBitString.append(twosComplement(opcodeMap[x.substr(0, stringPosition)])); //append label value

            } else {
              //register mode
              tempBitString.append("01");
              tempBitString.append(opcodeMap[x.substr(0, stringPosition)]); //append register value

            }


            x.erase(0, stringPosition+1);

          } else if (x.substr(4, 2) == "jl") {
            tempBitString.append(opcodeMap[x.substr(4, 2)]);
            x = x.substr(7, x.length() - 4);

            //std::cout << x << std::endl;
            if (opcodeMap.find(x.substr(0, stringPosition)) == opcodeMap.end()) { //enter if not found in map
              altStringPosition = stringPosition;
              stringPosition = x.find(" ");
             // std::cout << x.substr(0, stringPosition) << std::endl;
              if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                //indexed mode
                tempBitString.append("11"); //#(register) encountered
                //all space between whitespace and "(" (to isolate number)

                std::string z = x.substr(0, stringPosition);
                //int z_orig_length = x.substr(0, stringPosition).length();
                z.pop_back();
                z.pop_back();
                z.pop_back();
                z.pop_back();

                tempBitString.append(twosComplement(z));
                std::string y = x.substr(0, stringPosition);
                y.erase(0, z.length() + 1);
                y.pop_back();
                tempBitString.append(opcodeMap[y]);
                //x.erase(0, z_orig_length + 1);
//                  if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
//                    z = x.substr(0, stringPosition);
//                    z_orig_length = x.substr(0, stringPosition).length();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//
//                    tempBitString.append(twosComplement(z));
//                    y = x.substr(0, stringPosition);
//                    y.erase(0, z.length() + 1);
//                    y.pop_back();
//                    tempBitString.append(opcodeMap[y]);
//
//                  } //else {
//                    x.erase(0, z.length() + 1);
//                    x.pop_back();
//                    tempBitString.append(opcodeMap[x]);
//                  }

              } else if (x.substr(0, stringPosition).at(0) == '$') {
                //immediate mode
                tempBitString.append("00");
                std::string y = x.substr(1, x.length());
                if (opcodeMap.find(y) == opcodeMap.end()) { //can be label or number
                  tempBitString.append(twosComplement(y)); //number

                } else {
                  tempBitString.append(twosComplement(opcodeMap[y]));
                }

              }
            } else if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) { //if it's not in bitmap then it's a label, if it is then a register
              //direct mode
              tempBitString.append("10");
              tempBitString.append(twosComplement(opcodeMap[x.substr(0, stringPosition)])); //append label value

            } else {
              //register mode
              tempBitString.append("01");
              tempBitString.append(opcodeMap[x.substr(0, stringPosition)]); //append register value

            }


            x.erase(0, stringPosition+1);


          } else if (x.substr(4, 1) == "j") {
            tempBitString.append(opcodeMap[x.substr(4, 1)]);
            x = x.substr(6, x.length() - 4);
//std::cout << x << std::endl;
            if (opcodeMap.find(x.substr(0, stringPosition)) == opcodeMap.end()) { //enter if not found in map
              altStringPosition = stringPosition;
              stringPosition = x.find(" ");
             // std::cout << x.substr(0, stringPosition) << std::endl;
              if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
                //indexed mode
                tempBitString.append("11"); //#(register) encountered
                //all space between whitespace and "(" (to isolate number)

                std::string z = x.substr(0, stringPosition);
                //int z_orig_length = x.substr(0, stringPosition).length();
                z.pop_back();
                z.pop_back();
                z.pop_back();
                z.pop_back();

                tempBitString.append(twosComplement(z));
                std::string y = x.substr(0, stringPosition);
                y.erase(0, z.length() + 1);
                y.pop_back();
                tempBitString.append(opcodeMap[y]);
                //x.erase(0, z_orig_length + 1);
//                  if ((altStringPosition = (x.substr(0, stringPosition)).find(")")) != std::string::npos) {
//                    z = x.substr(0, stringPosition);
//                    z_orig_length = x.substr(0, stringPosition).length();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//                    z.pop_back();
//
//                    tempBitString.append(twosComplement(z));
//                    y = x.substr(0, stringPosition);
//                    y.erase(0, z.length() + 1);
//                    y.pop_back();
//                    tempBitString.append(opcodeMap[y]);
//
//                  } //else {
//                    x.erase(0, z.length() + 1);
//                    x.pop_back();
//                    tempBitString.append(opcodeMap[x]);
//                  }

              } else if (x.substr(0, stringPosition).at(0) == '$') {
                //immediate mode
                tempBitString.append("00");
                std::string y = x.substr(1, x.length());
                if (opcodeMap.find(y) == opcodeMap.end()) { //can be label or number
                  tempBitString.append(twosComplement(y)); //number

                } else {
                  tempBitString.append(twosComplement(opcodeMap[y]));
                }

              }
            } else if (bitMap.find(x.substr(0, stringPosition)) == bitMap.end()) { //if it's not in bitmap then it's a label, if it is then a register
              //direct mode
              tempBitString.append("10");
              tempBitString.append(twosComplement(opcodeMap[x.substr(0, stringPosition)])); //append label value

            } else {
              //register mode
              tempBitString.append("01");
              tempBitString.append(opcodeMap[x.substr(0, stringPosition)]); //append register value

            }


            x.erase(0, stringPosition+1);

          } else if (x.substr(4, 3) == "ret") {
            tempBitString.append(opcodeMap[x.substr(4, 3)]);

          } else if (x.substr(4, 3) == "nop") {
            tempBitString.append(opcodeMap[x.substr(4, 3)]);

          }

          int numZeroesForPadding;
          if ((8 - (tempBitString.size()) % 8) == 8) {
            numZeroesForPadding = 0;
          } else {
            numZeroesForPadding = (8 - (tempBitString.size()) % 8);
          }

          if (tempBitString.size() % 8 == 0) {
            eachOutputLine.push_back(tempBitString);
          } else {
            for (int i = 0; i < (numZeroesForPadding); i++) {
              tempBitString.append("0");
            }
            eachOutputLine.push_back(tempBitString);
          }
          tempBitString = "";
        }
      }

      std::string totalString;
      for (unsigned int i = 0; i < eachOutputLine.size(); i++) {
        totalString.append(eachOutputLine.at(i));
      }

      //std::cout << totalString.length() << std::endl;


      int byteCount = totalString.length() / 8;
      int biteSize =  totalString.length() / byteCount;
      int counter = 0;
      int stringLen = std::to_string(byteCount).length();

      for (unsigned int i = 0; i < totalString.length(); i++) {
        if (i % biteSize == 0) {
          if (i == 0) {
            std::cout << "Byte " << std::right << std::setw(stringLen) << std::fixed << i / 8 << ": ";
          } else {
            std::cout << std::endl << "Byte " << std::right << std::setw(stringLen) << std::fixed<< i / 8 << ": ";
          }
        }
        if (i == totalString.length() - 1) {
          std::cout << totalString.at(i) << std::endl;

        } else {
          std::cout << totalString.at(i);
        }


        counter++;
      }



      //have to create another map here
      //key = string that represents an instruction
      //value = actual opcode
      //once again parse for white space
      //have to do padding again (but will be easier because i know the values from previous map / bit counter
//TODO: start here ^^^^^^^^^^^

//check to see if two lines that don't start with a space are the same
      //if duplicate label is found when creating symbol table then print to std::cerr
      //std::cerr << "At least one duplicate label found.\n";


      //if we have successfully created a symbol table then we
      // will do a second pass to create machine code
      //to do this we will iterate through the vector and analyze each line to
      //determine what to print out

      //once i get the logic done i can work on formatting (like making
      //the numbers right-justified when printing out bytes)



//TODO: ---------------------------------------------------------------------------------



    } else { //if the file is not open
      std::cerr << "Failed to open file.\n";
      exit(0);
    }
    //close file
    ahrinFile.close();
  } else {   //input validation (wrong number of command line args)
    std::cerr << "Wrong number of arguments.\n";
    exit(0);
  }


  return 0;


  //forward references: iterate through file once and have a
  // byte counter to keep track of label values
  // then pass through one more time in order to convert assembly to machine code

  //If the wrong number of command-line arguments are provided, then the error message shown below must be printed to
  //standard error, and your program must exit with exit code 1.

  //you could check if
  //a line in an Ahrin assembly language program contains a label simply by checking if the first character is not a whitespace.

  //signed integers are represented in two's complement

//make sure to read directions carefully (there are some edge cases for wrong input)
}
//TODO: make part 1 code like part 2 for counting bits
