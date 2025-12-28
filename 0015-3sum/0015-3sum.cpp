//O(n^2)
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {

         vector<vector<int>> res;
        sort(nums.begin(), nums.end());
        //set<vector<int>> temp;
        for(int fix=0; fix<nums.size()-2; fix++){
            if (fix > 0 && nums[fix] == nums[fix - 1]) continue;
            int l=fix+1; int r=nums.size()-1;
            while(l<r){
                int sum=nums[fix]+nums[l]+nums[r];

                if(sum==0){
                    res.push_back({nums[fix],nums[l],nums[r]});
                    l++; r--;

                    while(l<r && nums[l] == nums[l-1]) l++;
                    while(l<r && nums[r]==nums[r+1]) r--;
                    
                }

                else if(sum<0){
                    l++;
                }

                else{
                    r--;
                }


            }

        }

    
        return res;

    }
};








// bruteforce O(n^3)
// class Solution {
// public:
//     vector<vector<int>> threeSum(vector<int>& nums) {
//         set<vector<int>> u;
        
        
//         int n=nums.size();
//         for(int fix=0; fix<nums.size()-2; fix++){
//             for(int l=fix+1; l<nums.size()-1; l++){
//                 for(int r=l+1; r<nums.size(); r++){
//                     if(nums[fix]+nums[l]+nums[r]==0){
//                         vector<int> trip = {nums[fix],nums[l],nums[r]};
//                         sort(trip.begin(), trip.end());
//                         u.insert(trip);
//                     }
//                 }
//             }
//         }
//         vector<vector<int>> res(u.begin(), u.end());
//         return res;



        
//     }
// };