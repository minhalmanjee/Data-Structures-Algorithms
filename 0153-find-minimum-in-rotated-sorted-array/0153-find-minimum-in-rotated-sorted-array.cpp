class Solution {
public:
    int findMin(vector<int>& nums) {
        int s=0; int e=nums.size()-1;
        int mi = INT_MAX;
        while(s<=e){
            int m= s+(e-s)/2;

            if(nums[s] <= nums[m]){
                mi = min(mi, nums[s]);
                s = m+1;
            }
            else{
                mi = (mi, nums[m]);
                e = m-1;
            }
        }
        return mi;
        
    }
};