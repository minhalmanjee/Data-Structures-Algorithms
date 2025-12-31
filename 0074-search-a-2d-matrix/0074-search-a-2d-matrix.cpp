class Solution {
public:

    bool searchinrow(vector<vector<int>>& matrix, int target, int row){
        int n = matrix[row].size()-1;
        int s = 0; int e = n;

        while(s<=e){
            int m = s+(e-s)/2;
            if(target == matrix[row][m]){
                return true;
            }
            else if(target < matrix[row][m]){
                e = m-1;
            }
            else{
                s=m+1;
            }
        }
        return false;
    }

    /// binary search to find the correct row, then we send row to fn above to apply BS to find target
// main func runs logn to find correct row. then only that roow sent to find target and that will take logm so total is logn+logm. by log property is becomes log(mxn)

    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int sr=0; int er= matrix.size()-1;
        int n = matrix[0].size()-1;
        while(sr <= er){
            int m= sr +(er-sr)/2;

            if(target >= matrix[m][0] && target <= matrix[m][n]){
                return searchinrow(matrix, target, m);
                //=> BS on this row
            }
            else if(target <= matrix[m][0]){
                er = m-1;
            }
            else{
                sr = m+1;
            }
        }
        return false;
}
};