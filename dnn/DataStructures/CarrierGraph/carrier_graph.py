# Input: List of rectangles (l, r, d, u)
# Output: Carrier graph G_x+

import matplotlib.pyplot as plt


def add_rectangle(V, E, l, r, d, u):
	V.add((l, d))
	V.add((l, u))
	V.add((r, d))
	V.add((r, u))
	E.add((l, d, l, u))
	E.add((r, d, r, u))
	E.add((l, d, r, d))
	E.add((l, u, r, u))


def carrier_graph(rectangles):
	V, E = set(), set()

	# Bounding box
	ll, rr, dd, uu = 10**9, -10**9, 10**9, -10**9
	for l, r, d, u in rectangles:
		ll = l if l < ll else ll
		rr = r if r > rr else rr
		dd = d if d < dd else dd
		uu = u if u > uu else uu
	ll, rr, dd, uu = ll-1, rr+1, dd-1, uu+1
	add_rectangle(V, E, ll, rr, dd, uu)

	# Each rectangles
	for l, r, d, u in rectangles:
		add_rectangle(V, E, l, r, d, u)

	# Right shadow
	for l, r, d, u in rectangles:
		rd, ru = (rr+1, 0, 0, 0), (rr+1, 0, 0, 0)
		for x1, y1, x2, y2 in E:
			if x1 == x2:
				if y1 <= d <= y2 and r < x1 < rd[0]:
					rd = (x1, y1, x2, y2)
				if y1 <= u <= y2 and r < x2 < ru[0]:
					ru = (x1, y1, x2, y2)

		if rd == ru:
			x1, y1, _, y2 = rd
			E.remove(rd)
			E.add((x1, y1, x1, d))
			E.add((x1, d, x1, u))
			E.add((x1, u, x1, y2))
			E.add((r, d, x1, d))
			E.add((r, u, x1, u))
			V.add((x1, d))
			V.add((x1, u))
		else:
			x1, y1, _, y2 = rd
			E.remove(rd)
			E.add((x1, y1, x1, d))
			E.add((x1, d, x1, y2))
			E.add((r, d, x1, d))
			V.add((x1, d))

			x1, y1, _, y2 = ru
			E.remove(ru)
			E.add((x1, y1, x1, u))
			E.add((x1, u, x1, y2))
			E.add((r, u, x1, u))
			V.add((x1, u))

	# Upper/Lower shadow
	for l, r, d, u in rectangles:
		ld, lu = (0, dd-1, 0, 0), (0, uu+1, 0, 0)
		for x1, y1, x2, y2 in E:
			if y1 == y2:
				if x1 <= l <= x2 and ld[1] < y1 < d:
					ld = (x1, y1, x2, y2)
				if x1 <= l <= x2 and u < y1 < lu[1]:
					lu = (x1, y1, x2, y2)

		x1, y1, x2, _ = ld
		E.remove(ld)
		E.add((x1, y1, l, y1))
		E.add((l, y1, x2, y1))
		E.add((l, y1, l, d))
		V.add((l, y1))

		x1, y1, x2, _ = lu
		E.remove(lu)
		E.add((x1, y1, l, y1))
		E.add((l, y1, x2, y1))
		E.add((l, u, l, y1))
		V.add((l, y1))

	# Now, V & E holds undirected carrier graph.
	Vd = {}
	for x, y in V:
		Vd[(x, y)] = set()
	for x1, y1, x2, y2 in E:
		if y1 == y2:
			# Horizontal edge
			assert x1 < x2
			Vd[(x1, y1)].add((x2, y2))
		elif x1 == x2:
			# Vertical edge
			assert y1 < y2
			if (x1 + 0.1, y1) not in Vd.keys():
				Vd[(x1 + 0.1, y1)] = set()
			if (x1 + 0.1, y2) not in Vd.keys():
				Vd[(x1 + 0.1, y2)] = set()

			Vd[(x1, y2)].add((x1, y1))
			Vd[(x1, y1)].add((x1 + 0.1, y1))
			Vd[(x1, y2)].add((x1 + 0.1, y2))
			Vd[(x1 + 0.1, y2)].add((x1 + 0.1, y1))
		else:
			assert False, 'Edge in E is not axis-parallel'

	return Vd


if __name__ == '__main__':
	Vd = carrier_graph([(1, 2, 3, 5), (3, 4, 2, 6), (5, 7, 1, 4)])
	for x1, y1 in Vd.keys():
		for x2, y2 in Vd[(x1, y1)]:
			plt.plot([x1, x2], [y1, y2])
	plt.show()
