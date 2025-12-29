class Solution {
public:
    int maxArea(vector<int>& height) {
        int l=0; int r=height.size()-1; int tmax=0;
        
        while(l<r){
            int fh = min(height[r], height[l]);
            int len = r-l;
            int a = len*fh;
            tmax = max(tmax, a);
            //getting rid of the smaller stick so if left smaller we do l++ and if right r--.
            if(height[l] < height[r]){
                l++;
            }
            else{
                r--;
            }
            
                       
        }

        
        return tmax;

       
    
    }
};