import numpy as np
from scipy.spatial import ConvexHull as CH
import matplotlib.pyplot as plt
import matplotlib.patches as patches

class SA:
	bound_lt=[-5,5]; bound_lb=[-5,-5]; bound_rt=[5,5]; bound_rb=[5,-5]
	
	def one_intersect(A,B):
		if A.left_top[0] < B.left_top[0] and B.left_top[0] < A.right_bot[0]:
			if not (B.left_bot[1] >= A.left_top[1] or B.left_top[1] <= A.left_bot[1]):
				return True

		elif A.left_top[0] < B.right_bot[0] and B.right_bot[0] < A.right_bot[0]:
			if not (B.left_bot[1] >= A.left_top[1] or B.left_top[1] <= A.left_bot[1]):
				return True
		elif A.left_bot[1] < B.left_bot[1] and B.left_bot[1] < A.left_top[1]:
			if not (B.left_bot[0] >= A.right_bot[0] or B.right_bot[0] <= A.left_bot[0]):
				return True

		elif A.left_bot[1] < B.left_top[1] and B.left_top[1] < A.left_top[1]:
			if not (B.left_bot[0] >= A.right_bot[0] or B.right_bot[0] <= A.left_bot[0]):
				return True

		elif A.left_top == B.left_top and A.left_bot == B.left_bot and A.right_top == B.right_top and A.right_bot == B.right_bot:
			return True
		else:
			return False

	def pdf_lb(A):
		A.move=A.tmove
		A.down_prob=1/5
		A.up_prob=2/5
		A.left_prob=1/5
		A.right_prob=2/5

	def pdf_lt(A):
		A.move=A.tmove
		A.down_prob=2/5
		A.up_prob=1/5
		A.left_prob=1/5
		A.right_prob=2/5

	def pdf_rt(A):
		A.move = A.tmove
		A.down_prob=2/5
		A.up_prob=1/5
		A.left_prob=2/5
		A.right_prob=1/5

	def pdf_rb(A):
		A.move = A.tmove
		A.down_prob=1/5
		A.up_prob=2/5
		A.left_prob=2/5
		A.right_prob=1/5

	def pdf_in(A):
		A.down_prob=1/3
		A.up_prob=1/3
		A.left_prob=1/3
		A.right_prob=1/3
		if A.move == A.tmove:
			A.move=A.move/3

	def pdf_change(A,b):
		change=False
		for i in A.get_coord():
			if i in b:
				change=True
				if A.left_top==i:
					SA.pdf_lt(A)
				elif A.left_bot==i:
					SA.pdf_lb(A)
				elif A.right_top==i:
					SA.pdf_rt(A)
				else:
					SA.pdf_rb(A)
		if not change:
			SA.pdf_in(A)

	def assign_label(state,b):
		for s in state:
			SA.pdf_change(s,b)

	def intersect(A,B):
		return (SA.one_intersect(A,B) or SA.one_intersect(B,A))

	def check_intersection(l):
		if len(l)==1:
			return False
		for i in range(len(l)):
			for j in range(len(l)-1-i):
				if SA.intersect(l[i],l[i+j+1]):
					return True
				else:
					pass
		return False

	def move(A):
		ver_amount=A.move*np.random.uniform(0,1)*np.random.uniform(0,1)
		par_amount=A.move*np.random.uniform(0,1)*np.random.uniform(0,1)
		temp1,temp2=np.random.uniform(0,1,2)
		if temp1 <= A.up_prob:
			y_ch = +ver_amount
			if A.left_top[1]+ver_amount >= SA.bound_lt[1]:
				if np.random.rand(1).item() > 0.5 and A.down_prob >= np.random.rand(1).item():
					y_ch=-y_ch
				else:
					y_ch=0
		elif A.up_prob < temp1 <= A.down_prob+A.up_prob:
			y_ch = -ver_amount
			if A.left_bot[1]-ver_amount <= SA.bound_lb[1]:
				if np.random.rand(1).item() > 0.5 and A.up_prob >= np.random.rand(1).item():
					y_ch=-y_ch
				else:
					y_ch=0
		else:
			y_ch = 0
		if temp2 <= A.left_prob:
			x_ch = -par_amount
			if A.left_top[0]-par_amount <= SA.bound_lt[0]:
				if np.random.rand(1).item() > 0.5 and A.right_prob >= np.random.rand(1).item():
					x_ch=-x_ch
				else:
					x_ch=0
		elif A.left_prob < temp2 <= A.left_prob+A.right_prob:
			x_ch = par_amount
			if A.right_top[0]+par_amount >= SA.bound_rt[0]:
				if np.random.rand(1).item() > 0.5 and A.left_prob >= np.random.rand(1).item():
					x_ch=-x_ch
				else:
					x_ch=0
		else:
			x_ch = 0
		A.coord([A.left_bot[0]+x_ch,A.left_bot[1]+y_ch])

	def get_point(l):
		points=[]
		for square in l:
			points+=square.get_coord()
		return points

	def objective_function(state):
		#convex hull area
		return CH(SA.get_point(state))

	def accept_move(cur_obj, next_obj,temper):
		if cur_obj > next_obj:
			return True
		elif temper > 0 and np.random.rand(1).item() >= np.exp(-(next_obj-cur_obj)/temper):
			return True
		else:
			return False

	def ojective_area(l):
		hull = CH(SA.get_point(l))
		return hull.area

	def CH_vertices(l):
		point=SA.get_point(l)
		hull = CH(point).vertices
		hull = hull.tolist()
		count=0
		result=[]
		while len(hull)!=0:
			if count in hull:
				result.append(point[count])
				hull.remove(count)
			count+=1
		return result

	def move_All(l):
		done=[]
		for i in l:
			while True:
				SA.move(i)
				done.append(i)
				if SA.check_intersection(done):
					done.pop(-1)
				else:
					break

	def SA_process(state, tem, i):
		temper=tem
		cur_state=state
		cur_hull=SA.objective_function(cur_state)
		cur_obj=cur_hull.area
		print(cur_obj)
		cur_optimal = cur_obj
		for t in range(temper+1):
			t-=1
			max_iter=i
			SA.assign_label(state,SA.CH_vertices(state))
			while True:
				max_iter-=1
				if t <= 0:
					break
				if max_iter <= 0:
					t=0
					break
				else:
					SA.move_All(state)
					next_state=state
					next_hull=SA.objective_function(next_state)
					next_obj=next_hull.area
					if SA.accept_move(cur_obj,next_obj,temper):
						if cur_optimal > next_obj:
							cur_optimal = next_obj
							SA.plot_state(next_state,t)

							plt.plot(np.array(SA.get_point(state))[cur_hull.vertices,0],np.array(SA.get_point(state))[cur_hull.vertices,1],'r--',lw=2)
						cur_state=next_state
						cur_hull=next_hull
						cur_obj=next_obj
						if t % 100 == 0:
							print(t,"번째 업데이트",cur_obj)
						break
					else:
						state=cur_state
		print(cur_optimal)
		print(np.array(SA.get_point(state))[cur_hull.vertices])
		plt.show()

	def plot_state(state,t):
		fig,ax=plt.subplots(1,1,figsize=(20,20))
		for p in [
			patches.Rectangle(i.left_bot,i.length,i.length)
			for i in state
		]:
			ax.add_patch(p)
		fig.savefig('rect%d.png'%t)

	def print_All(l):
		for i in l:
			print(i.get_coord())			

	class quadrangle:
		def __init__(self,xy,l):
			x,y=xy
			self.left_bot=xy
			self.left_top=[x,y+l]
			self.right_top=[x+l,y+l]
			self.right_bot=[x+l,y]
			self.length=l
			self.move=l
			self.tmove=l
			self.left_prob=1/3
			self.right_prob=1/3
			self.pstay_prob=1/3
			self.down_prob=1/3

		def length(self,l):
			tl=self.length
			self.length = l
			self.left_top[1]+= -tl+l
			self.right_top[0]+= -tl+l
			self.right_top[1]+= -tl+l
			self.right_bot[0]+=-tl+l

		def coord(self,lb):
			x,y=lb
			self.left_bot=lb
			self.left_top=[x,y+self.length]
			self.right_top=[x+self.length,y+self.length]
			self.right_bot=[x+self.length,y]

		def get_coord(self):
			return [self.left_bot,self.left_top,self.right_top,self.right_bot]

ql=[]
quad1 = SA.quadrangle([1,1],2)
ql.append(quad1)
quad2 = SA.quadrangle([3,3],2)
ql.append(quad2)
quad3 = SA.quadrangle([0,6],2)
ql.append(quad3)
quad4 = SA.quadrangle([5,2],2)
ql.append(quad4)

SA.SA_process(ql,1000,100)
