class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int n = nums.size();
        unordered_map<int, int> m;
        for(int i=0; i<=n; i++){
            if(m.count(target - nums[i])){
                return {m[target - nums[i]], i};
            }
            m[nums[i]] = i;

        }

        return {};
        
    }
};




// class Solution {
// public:
//     vector<int> twoSum(vector<int>& nums, int target) {
//         int n = nums.size();

//         vector<pair<int, int>> arr;
//         for (int i=0; i<n; i++){
//             arr.push_back({nums[i], i});
//         }

//         quicksort(arr, 0, n-1);

//         int l = 0; int r = n-1;
//         while(l<r){
//             int sum = arr[l].first + arr[r].first;
//             if(sum == target){
//                 return {arr[l].second, arr[r].second};
//             }
//             else if(sum < target){
//                 l++;
//             }
//             else{
//                 r--;
//             }
//         }
//         return {};        
//     }

//     void quicksort(vector<pair<int, int>>& arr, int l, int h){
//         if(l<h){
//             int p = partition(arr, l,h);
//             quicksort(arr, l, p-1);
//             quicksort(arr, p+1, h);
//         }
//     }

//     int partition(vector<pair<int, int>>& arr, int l, int h){
//         int pivot = arr[l].first;
//         int i = l; int j = h;
//         while(i<j){
//             while(i<=j && arr[i].first <= pivot){
//                 i++;
//             }
//             while(arr[j].first > pivot){
//                 j--;
//             }
//             if(i<j){
//                 swap(arr[i], arr[j]);
//             }
            
//         }
//         swap(arr[l], arr[j]);
//         return j;
//     }


// };




//Bruteforce
//class Solution {
// public:
//     vector<int> twoSum(vector<int>& nums, int target) {

//         for(int i=0; i<nums.size(); i++){
//             for(int j=0; j<nums.size(); j++){
//                 if(i==j){
//                     continue;
//                 }
//                 else{
//                     if(nums[i]+nums[j]==target){
//                         return {i,j};
//                     }
//                 }
//             }
//         }
//         return {};
        
//     }
// };