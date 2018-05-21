# Scala text analyzer

This code is for basic text parsing using Spark. The first part of the program counts the words and prints on the standard output the 5 words of more than 3 letters that have the highest occurrences in descending order for each text. The program considers uppercase and lowercase letters as being the same letter, and also ignore punctuation characters to count the occurrences (using replaceAll ("[,.!?:;]", "")). In the second part, the program compares the analyzes of the two texts, and prints in alphabetical order all the words that appear more than 100 times in each of the two texts.

To compile your program, you can use any computer with Spark and sbt and enter the following command in the program's own folder (containing the build.sbt file):
```
sbt package
```
```
spark−submit −−class Analisador \target/scala−2.11/analisador_2.11−0.1.jar \text1.txt text2.txt
```
