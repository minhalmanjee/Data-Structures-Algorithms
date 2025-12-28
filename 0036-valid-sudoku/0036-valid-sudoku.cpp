
//O(1) because 81 operations gauranteed so 81-constant it wont increase like O(n^2) since board size is always fixed.
class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {

        unordered_set<string> seen;
        for(int i=0; i<9; i++){
            for(int j=0; j<9; j++){

                if(board[i][j] == '.'){
                    continue;
                }

                char val = board[i][j];
                int box = (i/3) * 3 + (j/3);

                if(seen.count("r" + to_string(i)+val)||
                   seen.count("c" + to_string(j)+val)||
                   seen.count("b" + to_string(box)+val)){
                    return false;
                   }
                seen.insert("r"+ to_string(i)+val);
                seen.insert("c"+ to_string(j)+val);
                seen.insert("b"+ to_string(box)+val);
            }
        }
        return true;
    }
};










// Bruteforce: O(n^3)
// class Solution {
// public:
//     bool isValidSudoku(vector<vector<char>>& board) {
//         for(int i=0; i<9; i++){
//             for(int j=0; j<9; j++){
//                 if(board[i][j] == '.'){
//                     continue;
//                 }
//                 for(int k=j+1; k<9; k++){
//                     if(board[i][j] == board[i][k]){
//                         return false;
//                     }
//                 }
//             }
//         }

//         //cols

//         for(int j=0; j<9; j++){
//             for(int i=0; i<9; i++){
//                 if(board[i][j] == '.'){
//                     continue;
//                 }

//                 for(int k=i+1; k<9; k++){
//                     if(board[i][j] == board[k][j]){
//                         return false;
//                     }
//                 }
            
//             }

//         }

//         //boxes
//         for(int i=0; i<9; i+=3){
//             for(int j=0; j<9; j+=3){
//                 vector<int> cells;
//                 for(int a=0; a<3; a++){
//                     for(int b=0; b<3; b++){
//                         if(board[a+i][b+j] != '.'){
//                             cells.push_back(board[i+a][j+b]);
//                         }
//                     }
//                 }


//                 for(int x=0; x<cells.size(); x++){
//                     for(int y=x+1; y<cells.size(); y++){
//                         if(cells[x] == cells[y]){
//                             return false;
//                         }
//                     }
//                 }
//             }
//         }

//         return true;
        
//     }
// };