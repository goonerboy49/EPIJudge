void Backtrack(int a[], int offset, int n, int ans[][]) {
    if (isSolution(offset, n)) {
        ans.push_back(a);
        return;
    }

    // Generate all possible candidates for the next move
    int c[] = generateCandidates();

    offset = offset+1;
    for (int i = 0; i < c.size(); i++) {
        a[offset] = c[i];
        // make move
        Backtrack(a, offset, n, ans);
        // unmake move
        // If this is a search then finish early
        if (finished) {
            // set ans and return early.
        }
    }
}