class Solution {
public:
    long long zeroFilledSubarray(vector<int>& nums) {
        int count = 0;
        long long int subarrs = 0;
        for(int i=0; i<nums.size(); i++){
            if(nums[i] == 0){
                count += 1;
                subarrs += count;
            }
            if(nums[i] != 0){
                count = 0;
            }

            
        }

        return subarrs;
        
    }
};