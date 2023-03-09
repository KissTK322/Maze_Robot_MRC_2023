void BFS(int mat[][COL], Point src, Point dest)
{
    // check source and destination cell
    // of the matrix have value 1
    if (!mat[src.x][src.y] || !mat[dest.x][dest.y]) {
        cout << "No path found\n";
        return;
    }

    bool visited[ROW][COL];
    memset(visited, false, sizeof visited);

    // Mark the source cell as visited
    visited[src.x][src.y] = true;

    // Create a queue for BFS
    queue<queueNode> q;

    // Distance of source cell is 0
    queueNode s = {src, 0};
    q.push(s);  // Enqueue source cell

    // Store parent of each cell
    Point parent[ROW][COL];
    parent[src.x][src.y] = {-1, -1};

    // Do a BFS starting from source cell
    while (!q.empty())
    {
        queueNode curr = q.front();
        Point pt = curr.pt;

        // If we have reached the destination cell,
        // backtrack to source using parent info and print path
        if (pt.x == dest.x && pt.y == dest.y) {
            vector<Point> path;
            while (pt.x != -1 && pt.y != -1) {
                path.push_back(pt);
                pt = parent[pt.x][pt.y];
            }
            cout << "Shortest path: ";
            for (int i = path.size() - 1; i >= 0; i--) {
                cout << "(" << path[i].x << "," << path[i].y << ") ";
            }
            cout << "\n";
            return;
        }

        // Otherwise dequeue the front
        // cell in the queue
        // and enqueue its adjacent cells
        q.pop();

        for (int i = 0; i < 4; i++)
        {
            int row = pt.x + rowNum[i];
            int col = pt.y + colNum[i];

            // if adjacent cell is valid, has path and
            // not visited yet, mark it as visited, record
            // its parent, and enqueue it.
            if (isValid(row, col) && mat[row][col] &&
               !visited[row][col])
            {
                visited[row][col] = true;
                Point p = {pt.x, pt.y};
                parent[row][col] = p;
                queueNode Adjcell = { {row, col}, curr.dist + 1 };
                q.push(Adjcell);
            }
        }
    }

    // If we reach here, destination is not reachable
    cout << "No path found\n";
}
