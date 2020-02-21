using namespace std;
#include <vector>
vector<int> *stringToVector(string toConvert) {
  vector<int> *toReturn = new vector<int>();
  string substr = "";
  for (int i = 0; i < toConvert.size(); i++) {
    if (toConvert[i] == ',') {
      toReturn->push_back(stoi(substr));
      substr = "";
    } else {
      substr += toConvert[i];
    }
  }
  toReturn->push_back(stoi(substr));
  return toReturn;
}
int *getArrayFromVector(vector<int> in) {
  int numOfLayers = in.size();
  int *nil = new int[numOfLayers];
  for (int i = 0; i < numOfLayers; i++) {
    nil[i] = in[i];
  }
  return nil;
}