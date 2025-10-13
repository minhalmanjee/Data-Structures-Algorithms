class Solution {
public:
    bool increasingTriplet(vector<int>& nums) {
        int first = INT_MAX;   // smallest so far
        int second = INT_MAX;  // second smallest so far

        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] <= first) {
                first = nums[i];       // update smallest
            } else if (nums[i] <= second) {
                second = nums[i];      // update second smallest
            } else {
                // nums[i] > first and nums[i] > second → triplet exists
                return true;
            }
        }

        return false; // no triplet found
    }
};
