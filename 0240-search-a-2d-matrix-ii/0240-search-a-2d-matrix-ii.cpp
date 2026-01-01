class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        //rows => increasing order
        //cols => decreasing order
        // we choose mid as the box from where there is least number of outgoing ways like we can choose either corner, 2 outgoing ways only.

        //rows //cols
        int m = matrix.size(); int n=matrix[0].size();
        int r = 0; int c=n-1; //  this is mid- box with value 15 row =0 and col = n-1;

        while(r<m && c>=0){ 
            if(matrix[r][c] == target){
                return true;
            }else if(matrix[r][c] > target){
                c--;
            }else{
                r++;
            }


        }

        return false;



        
    }
};