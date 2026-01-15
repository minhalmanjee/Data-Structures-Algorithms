class Solution {
public:
    vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
        stack<int> s;
        unordered_map<int, int> m; //value, next greater
        int n = nums2.size();
        //m[n-1].push_back(-1);
        for(int i=n-1; i >= 0; i--){
            while(s.size() > 0 && s.top() <= nums2[i]){
                s.pop();
            }
            if(s.empty()){
                m[nums2[i]] = -1;
            }
            else{
                m[nums2[i]] = s.top();
            }
            s.push(nums2[i]);
        }

        vector<int> ans;
        for(int i=0; i<nums1.size(); i++){
            ans.push_back(m[nums1[i]]);
        }

        return ans;
        
    }
};












// Bruteforce
// class Solution {
// public:
//     vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
//         vector<int> ans; 
//         for(int i=0; i<nums1.size(); i++){
//             bool found=false;
//             for(int j=0; j<nums2.size(); j++){
//                 if(nums1[i] == nums2[j]){
//                     for(int k=j+1; k<nums2.size(); k++){
//                         if(nums2[k] > nums2[j]){
//                             ans.push_back(nums2[k]);
//                             found = true;
//                             break;
//                         }
//                     }
//                     break;                    
//                 }
                
//             }
//             if(!found) ans.push_back(-1);
//         }
//         return ans;
        
//     }
// };