class StockSpanner {
public:
    stack<pair<int,int>> s;
    int temp = 0; //temp, price
    StockSpanner() {
        
    }
    
    int next(int price) {

        temp++;
        int span = 1;
        while((s.size() > 0) && s.top().second <= price){
            s.pop();
        }
        if(s.size() == 0){
            span = temp;
        }
        else{
            span = temp-s.top().first;
        }
        
        s.push({temp, price});
        return span;
    }
};

/**
 * Your StockSpanner object will be instantiated and called as such:
 * StockSpanner* obj = new StockSpanner();
 * int param_1 = obj->next(price);
 */