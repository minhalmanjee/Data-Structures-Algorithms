class MinStack {
public:
    vector<int> st;
    vector<int> minst;

    MinStack() {}  // no need to redeclare vectors

    void push(int val) {
        st.push_back(val);
        if(minst.empty() || val <= minst.back()) {
            minst.push_back(val);
        }
    }

    void pop() {
        if(st.back() == minst.back()) {
            minst.pop_back();
        }
        st.pop_back();
    }

    int top() {
        return st.back();
    }

    int getMin() {
        return minst.back();
    }
};
