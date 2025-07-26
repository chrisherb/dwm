void
fibonacci(Monitor *m, int s)
{
	unsigned int i, n;
	int nx, ny, nw, nh;
	int gap, nrest = 0, wrest = 0, r = 1;
	Client *c;

	gap = m->gappx;

	// count windows
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;

	nx = m->wx + gap;
	ny = m->wy + gap;
	nw = m->ww - 2 * gap;
	nh = m->wh - 2 * gap;

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		if (r) {
			if ((i % 2 && (nh - gap) / 2 <= (bh + 2 * c->bw))
			    || (!(i % 2) && (nw - gap) / 2 <= (bh + 2 * c->bw))) {
				r = 0;
			}
			if (r && i < n - 1) {
				if (i % 2) {
					int nv = (nh - gap) / 2;
					nrest = nh - 2 * nv - gap;
					nh = nv;
				} else {
					int nv = (nw - gap) / 2;
					wrest = nw - 2 * nv - gap;
					nw = nv;
				}

				if ((i % 4) == 2 && !s)
					nx += nw + gap;
				else if ((i % 4) == 3 && !s)
					ny += nh + gap;
			}

			if ((i % 4) == 0) {
				if (s) {
					ny += nh + gap;
					nh += nrest;
				}
				else {
					nh -= nrest;
					ny -= nh + gap;
				}
			}
			else if ((i % 4) == 1) {
				nx += nw + gap;
				nw += wrest;
			}
			else if ((i % 4) == 2) {
				ny += nh + gap;
				nh += nrest;
				if (i < n - 1)
					nw += wrest;
			}
			else if ((i % 4) == 3) {
				if (s) {
					nx += nw + gap;
					nw -= wrest;
				} else {
					nw -= wrest;
					nx -= nw + gap;
					nh += nrest;
				}
			}
			if (i == 0) {
				if (n != 1) {
					nw = (m->ww - gap - 2 * gap) - (m->ww - gap - 2 * gap) * (1 - m->mfact);
					wrest = 0;
				}
				ny = m->wy + gap;
			}
			else if (i == 1)
				nw = m->ww - nw - gap - 2 * gap;
			i++;
		}
		resize(c, nx, ny, nw - (2 * c->bw), nh - (2 * c->bw), False);
	}
}

void
dwindle(Monitor *mon) {
	fibonacci(mon, 1);
}

void
spiral(Monitor *mon) {
	fibonacci(mon, 0);
}
