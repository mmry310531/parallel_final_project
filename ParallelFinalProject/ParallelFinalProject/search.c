//int search(int alpha, int beta, int depth)
//{
//	int i, j, x;
//	BOOL c, f;
//
//	/* we're as deep as we want to be; call quiesce() to get
//	   a reasonable score and return it. */
//	if (!depth)
//		return (*quiesce_func)(alpha, beta);
//
//#pragma omp atomic
//	++nodes;
//
//	/* do some housekeeping every 1024 nodes */
//	if ((nodes & 1023) == 0 && timeout())
//		return alpha;
//
//	pv_length[ply] = ply;
//
//	/* if this isn't the root of the search tree (where we have
//	   to pick a move and can't simply return 0) then check to
//	   see if the position is a repeat. if so, we can assume that
//	   this line is a draw and return 0. */
//	if (ply && reps())
//		return 0;
//
//	/* are we too deep? */
//	if (ply >= MAX_PLY - 1)
//		return (*eval_func)();
//	if (hply >= HIST_STACK - 1)
//		return (*eval_func)();
//
//	/* are we in check? if so, we want to search deeper */
//	c = in_check(side);
//	if (c)
//		++depth;
//
//	gen();
//
//	if (follow_pv)  /* are we following the PV? */
//		sort_pv();
//
//	f = FALSE;
//
//	/* loop through the moves */
//	for (i = branchNodes[ply]; i < branchNodes[ply + 1]; ++i) {
//		sort(i);
//		if (!makemove(MoveSet[i].m.b))
//			continue;
//		f = TRUE;
//		x = -search(-beta, -alpha, depth - 1);
//		takeback();
//		if (stop_search)
//			return alpha;
//		if (x > alpha) {
//
//			/* this move caused a cutoff, so increase the history
//			   value so it gets ordered high next time we can
//			   search it */
//			history[(int)MoveSet[i].m.b.from][(int)MoveSet[i].m.b.to] += depth;
//			if (x >= beta)
//				return beta;
//			alpha = x;
//
//			/* update the PV */
//			pv[ply][ply] = MoveSet[i].m;
//			for (j = ply + 1; j < pv_length[ply + 1]; ++j)
//				pv[ply][j] = pv[ply + 1][j];
//			pv_length[ply] = pv_length[ply + 1];
//		}
//	}
//
//	/* no legal moves? then we're in checkmate or stalemate */
//	if (!f) {
//		if (c)
//			return -10000 + ply;
//		else
//			return 0;
//	}
//
//	/* fifty move draw rule */
//	if (fifty >= 100)
//		return 0;
//	return alpha;
//}