from stl import mesh
import numpy as np
import numpy.linalg as la
from scipy.spatial import Delaunay
from scipy.spatial import ConvexHull
import timeit
import copy
import math
import random

np.seterr(divide='ignore', invalid='ignore')

def direction_to_bone(cog,bv): # cog는 2-dim vector (x,y)
	center=cog
	dist_list=[]
	for i in range(len(bv)):
		dist = la.norm(center-bv[i])
		dist_list.append([dist,i])
	dist_list.sort()
	p1,p2=np.array(bv[dist_list[0][1]]),np.array(bv[dist_list[1][1]])
	d=abs(np.cross(p2-p1,center-p1)/la.norm(p2-p1))
	original=p1-p2
	dir_vector1=np.array([original[1],-original[0]])
	dir_vector1=dir_vector1/la.norm(dir_vector1)
	dir_vector2=np.array([-original[1],original[0]])
	dir_vector2=dir_vector2/la.norm(dir_vector2)
	if abs(np.cross(p2-p1,center+dir_vector1*d-p1)/la.norm(p2-p1)) < abs(np.cross(p2-p1,center+dir_vector2*d-p1)/la.norm(p2-p1)):
		dir_vector = dir_vector1
		center=center+dir_vector1*d
	else:
		dir_vector = dir_vector2
		center=center+dir_vector2*d
	dir_vector=dir_vector.tolist()
	dir_vector.append(0.0)
	dir_vector=np.array(dir_vector)
	dir_list=[dir_vector,np.array([dir_vector[1],-dir_vector[0],0.0]),np.array([-dir_vector[1],dir_vector[0],0.0])]
	return d,dir_list

def get_tooth_list():
	tooth_list=[]
	for _ in range(16):
		_+=1
		temp = mesh.Mesh.from_file('./new teeth/case3/st'+str(_)+'.stl')
		tooth_list.append(temp)
	return tooth_list

def get_2d_hull(bound):
	vertex=[]
	for i in bound.vectors:
		temp = np.matrix(i).tolist()
		v1 = temp[0]
		v2 = temp[1]
		v3 = temp[2]
		for j in [v1,v2,v3]:
			tj = [j[0],j[1]]
			if tj in vertex:
				pass
			else:
				vertex.append(tj)
	vertex=np.array(vertex)
	hull = Delaunay(vertex)
	return hull

def get_2d_vertices(tooth):
	vertices=[]
	for i in tooth.vectors:
		temp = np.matrix(i).tolist()
		v1 = temp[0]
		v2 = temp[1]
		v3 = temp[2]
		for j in [v1,v2,v3]:
			tj = [j[0],j[1]]
			if tj in vertices:
				pass
			else:
				vertices.append(tj)
	return vertices

def get_3d_vertices(tooth):
	vertices=[]
	for i in tooth.vectors:
		temp = np.matrix(i).tolist()
		v1 = temp[0]
		v2 = temp[1]
		v3 = temp[2]
		for j in [v1,v2,v3]:
			if j in vertices:
				pass
			else:
				vertices.append(j)
	return vertices

def get_3d_2d_vertices(tooth):
	vertices_3d = []
	vertices_2d = []
	for i in tooth.vectors:
		temp = np.matrix(i).tolist()
		v1 = temp[0]
		v2 = temp[1]
		v3 = temp[2]
		for j in [v1,v2,v3]:
			if j in vertices_3d:
				pass
			else:
				vertices_3d.append(j)
			if j[:2] in vertices_2d:
				pass
			else:
				vertices_2d.append(j[:2])
	return vertices_3d,vertices_2d

def get_center(teeth):
	volume,cog,inertia  = teeth.get_mass_properties()
	return cog

def get_2d_vertices_of_bone(bone):
	vertices=[]
	for i in bone.vectors:
		temp = np.matrix(i).tolist()
		v1 = temp[0]
		v2 = temp[1]
		v3 = temp[2]
		for j in [v1,v2,v3]:
			tj = [j[0],j[1]]
			if tj in vertices:
				pass
			else:
				vertices.append(tj)
	vertices.sort()
	print("왼쪽 끝 : ",vertices[0], ", 오른쪽 끝 : ",vertices[-1])
	return vertices

def get_ch_of_teeth(tv_l):
	ch_list=[]
	for v in tv_l:
		ch_list.append(ConvexHull(v))
	ch_vlist=[]
	for j in range(len(tv_l)):
		hv=[]
		for i in ch_list[j].simplices:
			if i[0] in hv:
				pass
			else:
				hv.append(i[0])
			if i[1]  in hv:
				pass
			else:
				hv.append(i[1])
		ch_vertices=[]
		for i in hv:
			ch_vertices.append(tv_l[j][i])
		ch_vlist.append(np.array(ch_vertices))
	return ch_vlist

def find_distance_btw_teeth(teeth_vertices):
	ch_vlist=get_ch_of_teeth(teeth_vertices)
	dist_btw_hull=[]
	for i in range(len(ch_vlist)-1):
		dist1=distance_btw_ch(ch_vlist[i],ch_vlist[i+1])
		dist2=distance_btw_ch(ch_vlist[i+1],ch_vlist[i])
		if dist1 < dist2:
			dist_btw_hull.append(dist1)
		else:
			dist_btw_hull.append(dist2)
	return dist_btw_hull

def distance_btw_ch(ch1,ch2):
	min_dist=math.inf
	for i in ch1:
		for j in range(len(ch2)):
			if j < len(ch2)-1:
				d=distance_btw_line_point(i,ch2[j],ch2[j+1])
				if min_dist>d:
					min_dist=d
			else:
				d=distance_btw_line_point(i,ch2[j],ch2[0])
				if min_dist>d:
					min_dist=d
	return min_dist

def distance_btw_line_point(p,l0,l1):
	t = ((p[0]-l0[0])*(l1[0]-l0[0])+(p[1]-l0[1])*(l1[1]-l0[1]))/la.norm(l0-l1)
	t = max(0, min(1,t))
	return la.norm(p-np.array([l0[0]+t*(l1[0]-l0[0]),l0[1]+t*(l1[1]-l0[1])]))


def polygon_intersection(p1,p2):
	p1_hull=Delaunay(p1)
	p2_hull=Delaunay(p2)
	isin_p1=p1_hull.find_simplex(p2)>=0
	isin_p2=p2_hull.find_simplex(p1)>=0
	for _ in isin_p1:
		if _:
			return True
	for _ in isin_p2:
		if _:
			return True
	return False


def objective_f_2d(tlist,bvlist):
	mean_list=[]
	for i in tlist:
		mean_list.append(i.mean(axis=0))
	from_anchor=[]
	for i in range(len(mean_list)):
		dist=distance_from_bone(bvlist,mean_list[i])
		from_anchor.append(dist)
	from_anchor=np.array(from_anchor)
	from_anchor*=len(tlist)
	sum_dist=0
	for i in range(len(mean_list)-1):
		sum_dist+=la.norm(mean_list[i]-mean_list[i+1])
	total_objective_value=from_anchor.sum()+10*sum_dist
	return total_objective_value


def SA_process_3d():
	global left_right
	global to_bone_far
	global to_bone_close
	global rotation_angle
	global max_iter # 10에서 바꿔봤음
	global max_temper
	global initial_c
	global distance_bt_weight
	global distance_fb_weight
	global different_ang_weight
	global from_bone_far
	global from_bone_close
	global to_bone_far_prob
	global to_bone_close_prob
	global left_right_far_prob
	global left_right_close_prob
	global from_bone_far_prob
	global from_bone_close_prob
	global stay_far_prob
	global stay_close_prob
	global to_bone_rotation_prob
	global from_bone_rotation_prob
	global stay_roatation_prob
	global position_weight

	print("left_right : ",left_right, ", to_bone_close : ",to_bone_close,", to_bone_far : ",to_bone_far,", rotation_angle : ",rotation_angle)
	start=timeit.default_timer()
	tooth_list=get_tooth_list()
	bvlist=get_2d_vertices_of_bone(mesh.Mesh.from_file('./new teeth/case3/bone.stl'))
	origin_center=[]
	temp_list=[] 
	temp_list_2d=[]
	for i in tooth_list:
		center=get_center(i)
		vertex_3d = np.array(get_3d_vertices(i))
		vertex_2d = np.array(get_2d_vertices(i))
		origin_center.append(np.array(center))
		temp_list.append(vertex_3d)
		temp_list_2d.append(np.array(vertex_2d))
	best_center=copy.deepcopy(origin_center) 
	moving_center=copy.deepcopy(origin_center)
	best_2d_list = copy.deepcopy(temp_list_2d)

	dist_btw_teeth = find_distance_btw_teeth(temp_list_2d)
	dist_btw_teeth_scr = sum(dist_btw_teeth)
	dist_list=[]
	dir_list=[]
	for i in range(len(temp_list_2d)):
		dist_dir = direction_to_bone(origin_center[i][:2],bvlist)
		dist_list.append(dist_dir[0])
		dir_list.append(dist_dir[1][:])
	dist_from_bone_scr = sum(dist_list)

	norm_list=[]
	for i in tooth_list:
		initial_norm=i.normals[-10]
		initial_norm=initial_norm/la.norm(initial_norm)
		norm_list.append(initial_norm)
	diff_angle_scr = 0
	correct_angle = np.full((len(temp_list),3),np.array([0,0,-1]),dtype=object)
	for i in range(len(temp_list)):
		diff_angle_scr += np.arccos(np.dot(correct_angle[i],norm_list[i]))

	right_position = {}
	important_teeth = [0,5,7,8,10,15]
	for i in important_teeth:
		right_position[i]=bvlist[i]
	teeth_position_scr = 0
	for i in important_teeth:
		teeth_position_scr += la.norm(origin_center[i][:2]-right_position[i])

	best_score = distance_fb_weight*dist_from_bone_scr+distance_bt_weight*dist_btw_teeth_scr+different_ang_weight*diff_angle_scr+position_weight*teeth_position_scr
	print("실행전 점수 : ",best_score)
	improved=0
	nothing=0
	change_count=0
	for c in range(initial_c):
		temper=max_temper/(1+np.sqrt(1+c))
		iteration=0
		good=True
		while True:
			iteration+=1
			if iteration > max_iter:
				print(temper,"이번 turn에서 이웃해 못찾음  score = ",best_score, ", number of iteration = ",c, "  , 최근 개선되지 않은 횟수 : ",nothing)
				good=False
				break
			next_stage=[0]*len(temp_list_2d)
			move_history=[1 for _ in range(len(temp_list_2d))]
			for i in range(len(temp_list_2d)):
				if i == 0:
					if dist_list[i] > 0.01:
						temp_direction=dir_list[i]
						rannum=np.random.rand()
						if dist_btw_teeth[i] > 0.0005:
							if rannum < stay_far_prob:
								next_stage[i]=np.zeros(3)
								move_history[i]=2
							elif rannum < (stay_far_prob+left_right_far_prob): next_stage[i]=temp_direction[1]*left_right 
							elif rannum < (stay_far_prob+left_right_far_prob*2): next_stage[i]=temp_direction[2]*left_right
							elif rannum < (stay_far_prob+left_right_far_prob*2+from_bone_far_prob): 
								next_stage[i]=-temp_direction[0]*from_bone_far 
								move_history[i]=3
							else: next_stage[i]=temp_direction[0]*to_bone_far
						else:
							if rannum < stay_far_prob:
								next_stage[i]=np.zeros(3)
								move_history[i]=2
							elif rannum < (stay_far_prob+left_right_far_prob): 
								next_stage[i]=np.zeros(3)
								move_history[i]=2
							elif rannum < (stay_far_prob+left_right_far_prob*2): next_stage[i]=temp_direction[2]*left_right*random.choice([1,1.5,2,2.5])
							elif rannum < (stay_far_prob+left_right_far_prob*2+from_bone_far_prob): 
								next_stage[i]=-temp_direction[0]*from_bone_far 
								move_history[i]=3
							else: next_stage[i]=temp_direction[0]*to_bone_far
					else:
						temp_direction=dir_list[i] 
						rannum=np.random.rand()
						if dist_btw_teeth[i] > 0.0005:
							if rannum < to_bone_close_prob: next_stage[i]=temp_direction[0]*to_bone_close
							elif rannum < (to_bone_close_prob+from_bone_close_prob): 
								next_stage[i]=-temp_direction[0]*from_bone_close 
								move_history[i]=3
							elif rannum <(to_bone_close_prob+from_bone_close_prob+stay_close_prob): 
								next_stage[i]=np.zeros(3)
								move_history[i]=2
							elif rannum < (to_bone_close_prob+from_bone_close_prob+stay_close_prob+left_right_close_prob): next_stage[i]=temp_direction[1]*left_right
							else: next_stage[i]=temp_direction[2]*left_right
						else:
							if rannum < to_bone_close_prob: next_stage[i]=temp_direction[0]*to_bone_close
							elif rannum < (to_bone_close_prob+from_bone_close_prob): 
								next_stage[i]=-temp_direction[0]*from_bone_close
								move_history[i]=3
							elif rannum <(to_bone_close_prob+from_bone_close_prob+stay_close_prob): 
								next_stage[i]=np.zeros(3)
								move_history[i]=2
							elif rannum < (to_bone_close_prob+from_bone_close_prob+stay_close_prob+left_right_close_prob):
								next_stage[i]=np.zeros(3)
								move_history[i]=2
							else: next_stage[i]=temp_direction[2]*left_right
				elif i == 15:
					if dist_list[i] > 0.01:
						temp_direction=dir_list[i] 
						rannum=np.random.rand()
						if dist_btw_teeth[i-1] > 0.0005:
							if rannum < stay_far_prob:
								next_stage[i]=np.zeros(3)
								move_history[i]=2
							elif rannum < (stay_far_prob+left_right_far_prob): next_stage[i]=temp_direction[1]*left_right
							elif rannum < (stay_far_prob+left_right_far_prob*2): next_stage[i]=temp_direction[2]*left_right
							elif rannum < (stay_far_prob+left_right_far_prob*2+from_bone_far_prob): 
								next_stage[i]=-temp_direction[0]*from_bone_far
								move_history[i]=3
							else: next_stage[i]=temp_direction[0]*to_bone_far
						else:
							if rannum < stay_far_prob:
								next_stage[i]=np.zeros(3)
								move_history[i]=2
							elif rannum < (stay_far_prob+left_right_far_prob): 
								next_stage[i]=np.zeros(3)
								move_history[i]=2
							elif rannum < (stay_far_prob+left_right_far_prob*2): next_stage[i]=temp_direction[1]*left_right*random.choice([1,1.5,2,2.5])
							elif rannum < (stay_far_prob+left_right_far_prob*2+from_bone_far_prob): 
								next_stage[i]=-temp_direction[0]*from_bone_far
								move_history[i]=3
							else: next_stage[i]=temp_direction[0]*to_bone_far
					else:
						temp_direction=dir_list[i]
						rannum=np.random.rand()
						if dist_btw_teeth[i-1] > 0.0005:
							if rannum < to_bone_close_prob: next_stage[i]=temp_direction[0]*to_bone_close
							elif rannum < (to_bone_close_prob+from_bone_close_prob): 
								next_stage[i]=-temp_direction[0]*from_bone_close 
								move_history[i]=3
							elif rannum <(to_bone_close_prob+from_bone_close_prob+stay_close_prob): 
								next_stage[i]=np.zeros(3)
								move_history[i]=2
							elif rannum < (to_bone_close_prob+from_bone_close_prob+stay_close_prob+left_right_close_prob): next_stage[i]=temp_direction[1]*left_right
							else: next_stage[i]=temp_direction[2]*left_right
						else:
							if rannum < to_bone_close_prob: next_stage[i]=temp_direction[0]*to_bone_close
							elif rannum < (to_bone_close_prob+from_bone_close_prob): 
								next_stage[i]=-temp_direction[0]*from_bone_close 
								move_history[i]=3
							elif rannum <(to_bone_close_prob+from_bone_close_prob+stay_close_prob): 
								next_stage[i]=np.zeros(3)
								move_history[i]=2
							elif rannum < (to_bone_close_prob+from_bone_close_prob+stay_close_prob+left_right_close_prob):
								next_stage[i]=np.zeros(3)
								move_history[i]=2
							else: next_stage[i]=temp_direction[1]*left_right
				else:
					if dist_list[i] > 0.01:
						temp_direction=dir_list[i] 
						rannum=np.random.rand()
						if dist_btw_teeth[i] > 0.0005:
							if dist_btw_teeth[i-1] > 0.0005:
								if rannum < stay_far_prob:
									next_stage[i]=np.zeros(3)
									move_history[i]=2
								elif rannum < (stay_far_prob+left_right_far_prob): next_stage[i]=temp_direction[1]*left_right 
								elif rannum < (stay_far_prob+left_right_far_prob*2): next_stage[i]=temp_direction[2]*left_right
								elif rannum < (stay_far_prob+left_right_far_prob*2+from_bone_far_prob): 
									next_stage[i]=-temp_direction[0]*from_bone_far 
									move_history[i]=3
								else: next_stage[i]=temp_direction[0]*to_bone_far
							else:
								if rannum < stay_far_prob:
									next_stage[i]=np.zeros(3)
									move_history[i]=2
								elif rannum < (stay_far_prob+left_right_far_prob): next_stage[i]=temp_direction[1]*left_right
								elif rannum < (stay_far_prob+left_right_far_prob*2):
									next_stage[i]=np.zeros(3)
									move_history[i]=2
								elif rannum < (stay_far_prob+left_right_far_prob*2+from_bone_far_prob): 
									next_stage[i]=-temp_direction[0]*from_bone_far 
									move_history[i]=3
								else: next_stage[i]=temp_direction[0]*to_bone_far 
						else:
							if dist_btw_teeth[i-1] > 0.005:
								if rannum < stay_far_prob:
									next_stage[i]=np.zeros(3)
									move_history[i]=2
								elif rannum < (stay_far_prob+left_right_far_prob): 
									next_stage[i]=np.zeros(3)
									move_history[i]=2
								elif rannum < (stay_far_prob+left_right_far_prob*2): next_stage[i]=temp_direction[2]*left_right
								elif rannum < (stay_far_prob+left_right_far_prob*2+from_bone_far_prob): 
									next_stage[i]=-temp_direction[0]*from_bone_far
									move_history[i]=3
								else: next_stage[i]=temp_direction[0]*to_bone_far
							else:
								if rannum < stay_far_prob:
									next_stage[i]=np.zeros(3)
									move_history[i]=2
								elif rannum < (stay_far_prob+left_right_far_prob): 
									next_stage[i]=np.zeros(3)
									move_history[i]=2
								elif rannum < (stay_far_prob+left_right_far_prob*2):
									next_stage[i]=np.zeros(3)
									move_history[i]=2
								elif rannum < (stay_far_prob+left_right_far_prob*2+from_bone_far_prob): 
									next_stage[i]=-temp_direction[0]*from_bone_far
									move_history[i]=3
								else: next_stage[i]=temp_direction[0]*to_bone_far 
					else:
						temp_direction=dir_list[i]
						rannum=np.random.rand()
						if dist_btw_teeth[i] > 0.0005:
							if dist_btw_teeth[i-1] > 0.0005:
								if rannum < to_bone_close_prob: next_stage[i]=temp_direction[0]*to_bone_close
								elif rannum < (to_bone_close_prob+from_bone_close_prob): 
									next_stage[i]=-temp_direction[0]*from_bone_close 
									move_history[i]=3
								elif rannum <(to_bone_close_prob+from_bone_close_prob+stay_close_prob): 
									next_stage[i]=np.zeros(3)
									move_history[i]=2
								elif rannum < (to_bone_close_prob+from_bone_close_prob+stay_close_prob+left_right_close_prob): next_stage[i]=temp_direction[1]*left_right 
								else: next_stage[i]=temp_direction[2]*left_right
							else:
								if rannum < to_bone_close_prob: next_stage[i]=temp_direction[0]*to_bone_close
								elif rannum < (to_bone_close_prob+from_bone_close_prob): 
									next_stage[i]=-temp_direction[0]*from_bone_close 
									move_history[i]=3
								elif rannum <(to_bone_close_prob+from_bone_close_prob+stay_close_prob): 
									next_stage[i]=np.zeros(3)
									move_history[i]=2
								elif rannum < (to_bone_close_prob+from_bone_close_prob+stay_close_prob+left_right_close_prob): next_stage[i]=temp_direction[1]*left_right 
								else:
									next_stage[i]=np.zeros(3)
									move_history[i]=2									
						else:
							if dist_btw_teeth[i-1] > 0.0005:
								if rannum < to_bone_close_prob: next_stage[i]=temp_direction[0]*to_bone_close
								elif rannum < (to_bone_close_prob+from_bone_close_prob): 
									next_stage[i]=-temp_direction[0]*from_bone_close 
									move_history[i]=3
								elif rannum <(to_bone_close_prob+from_bone_close_prob+stay_close_prob): 
									next_stage[i]=np.zeros(3)
									move_history[i]=2
								elif rannum < (to_bone_close_prob+from_bone_close_prob+stay_close_prob+left_right_close_prob):
									next_stage[i]=np.zeros(3)
									move_history[i]=2
								else: next_stage[i]=temp_direction[1]*left_right
							else:
								if rannum < to_bone_close_prob: next_stage[i]=temp_direction[0]*to_bone_close
								elif rannum < (to_bone_close_prob+from_bone_close_prob): 
									next_stage[i]=-temp_direction[0]*from_bone_close 
									move_history[i]=3
								elif rannum <(to_bone_close_prob+from_bone_close_prob+stay_close_prob): 
									next_stage[i]=np.zeros(3)
									move_history[i]=2
								elif rannum < (to_bone_close_prob+from_bone_close_prob+stay_close_prob+left_right_close_prob):
									next_stage[i]=np.zeros(3)
									move_history[i]=2
								else:
									next_stage[i]=np.zeros(3)
									move_history[i]=2

			move_temp=[]
			center_temp=[]
			for j in range(len(temp_list_2d)):
				one_temp=[] 
				for k in temp_list[j]:
					one_temp.append(k+next_stage[j])
				move_temp.append(np.array(one_temp))
				center_temp.append(moving_center[j]+next_stage[j])
			done=True

			x_rotation_list=[]
			z_rotation_list=[]
			z_rev_rotation_list=[]
			for i in range(len(temp_list_2d)):
				if move_history[i] == 1:
					rannum=np.random.rand()
					if rannum < to_bone_rotation_prob: 
						angle_diff = np.arccos(np.dot(np.array([0,0,-1]),norm_list[i])) 
						if  angle_diff > 0.02: 
							if  norm_list[i][0]>= 0:
								temp_x = 1
							else:
								temp_x = -1

							if  norm_list[i][1]>= 0:
								temp_y = 1
							else:
								temp_y = -1

							xy_norm = norm_list[i][:2]
							xy_norm = xy_norm/la.norm(xy_norm)
							if temp_y > 0:
								xy_target = np.array([0,1])
								z_angle = np.arccos(np.dot(xy_target,xy_norm))
								if temp_x  > 0:
									pass
								else:
									z_angle = -z_angle
							else:
								xy_target = np.array([0,-1])
								z_angle = np.arccos(np.dot(xy_target,xy_norm))
								if temp_x  > 0:
									z_angle = -z_angle
								else:
									pass
							rotation_z = np.array([[np.cos(z_angle),-np.sin(z_angle),0],[np.sin(z_angle),np.cos(z_angle),0],[0,0,1]]) 
							for v in range(len(move_temp[i])):
								move_temp[i][v] -= center_temp[i]
								move_temp[i][v] = np.dot(rotation_z,move_temp[i][v])

							temp_norm = np.dot(rotation_z,norm_list[i])
							yz_norm = temp_norm[1:]
							yz_norm = yz_norm/la.norm(yz_norm)
							yz_target = np.array([0,-1])
							x_angle = rotation_angle 
							if temp_y > 0:
								x_angle=-x_angle
							else:
								pass

							rev_rotation_z = np.array([[np.cos(-z_angle),-np.sin(-z_angle),0],[np.sin(-z_angle),np.cos(-z_angle),0],[0,0,1]]) #z_axis
							rotation_x = np.array([[1,0,0],[0,np.cos(x_angle),-np.sin(x_angle)],[0,np.sin(x_angle),np.cos(x_angle)]]) #x_axis
							for v in range(len(move_temp[i])):
								move_temp[i][v] = np.dot(rotation_x,move_temp[i][v])
								move_temp[i][v] = np.dot(rev_rotation_z,move_temp[i][v])						
								move_temp[i][v] += center_temp[i]
							x_rotation_list.append(rotation_x)
							z_rotation_list.append(rotation_z)
							z_rev_rotation_list.append(rev_rotation_z)				
						else:
							x_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
							z_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
							z_rev_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
					else:
						x_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
						z_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
						z_rev_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
				elif move_history[i] == 2:
					rannum=np.random.rand()
					if rannum < stay_roatation_prob: 
						angle_diff = np.arccos(np.dot(np.array([0,0,-1]),norm_list[i]))
						if  angle_diff > 0.01:       
							if  norm_list[i][0]>= 0:
								temp_x = 1
							else:
								temp_x = -1

							if  norm_list[i][1]>= 0:
								temp_y = 1
							else:
								temp_y = -1

							xy_norm = norm_list[i][:2]
							xy_norm = xy_norm/la.norm(xy_norm)
							if temp_y > 0:
								xy_target = np.array([0,1])
								z_angle = np.arccos(np.dot(xy_target,xy_norm))
								if temp_x  > 0:
									pass
								else:
									z_angle = -z_angle
							else:
								xy_target = np.array([0,-1])
								z_angle = np.arccos(np.dot(xy_target,xy_norm))
								if temp_x  > 0:
									z_angle = -z_angle
								else:
									pass
							rotation_z = np.array([[np.cos(z_angle),-np.sin(z_angle),0],[np.sin(z_angle),np.cos(z_angle),0],[0,0,1]]) 
							for v in range(len(move_temp[i])):
								move_temp[i][v] -= center_temp[i]
								move_temp[i][v] = np.dot(rotation_z,move_temp[i][v])

							temp_norm = np.dot(rotation_z,norm_list[i])
							yz_norm = temp_norm[1:]
							yz_norm = yz_norm/la.norm(yz_norm)
							yz_target = np.array([0,-1])
							x_angle = rotation_angle 
							if temp_y > 0:
								x_angle=-x_angle
							else:
								pass

							rev_rotation_z = np.array([[np.cos(-z_angle),-np.sin(-z_angle),0],[np.sin(-z_angle),np.cos(-z_angle),0],[0,0,1]]) 
							rotation_x = np.array([[1,0,0],[0,np.cos(x_angle),-np.sin(x_angle)],[0,np.sin(x_angle),np.cos(x_angle)]]) 
							for v in range(len(move_temp[i])):
								move_temp[i][v] = np.dot(rotation_x,move_temp[i][v])
								move_temp[i][v] = np.dot(rev_rotation_z,move_temp[i][v])						
								move_temp[i][v] += center_temp[i]
							x_rotation_list.append(rotation_x)
							z_rotation_list.append(rotation_z)
							z_rev_rotation_list.append(rev_rotation_z)				
						else:
							x_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
							z_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
							z_rev_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
					else:
						x_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
						z_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
						z_rev_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
				else:
					rannum=np.random.rand()
					if rannum < from_bone_rotation_prob:
						angle_diff = np.arccos(np.dot(np.array([0,0,-1]),norm_list[i]))
						if  angle_diff > 0.01:          
							if  norm_list[i][0]>= 0:
								temp_x = 1
							else:
								temp_x = -1

							if  norm_list[i][1]>= 0:
								temp_y = 1
							else:
								temp_y = -1

							xy_norm = norm_list[i][:2]
							xy_norm = xy_norm/la.norm(xy_norm)
							if temp_y > 0:
								xy_target = np.array([0,1])
								z_angle = np.arccos(np.dot(xy_target,xy_norm))
								if temp_x  > 0:
									pass
								else:
									z_angle = -z_angle
							else:
								xy_target = np.array([0,-1])
								z_angle = np.arccos(np.dot(xy_target,xy_norm))
								if temp_x  > 0:
									z_angle = -z_angle
								else:
									pass
							rotation_z = np.array([[np.cos(z_angle),-np.sin(z_angle),0],[np.sin(z_angle),np.cos(z_angle),0],[0,0,1]])
							for v in range(len(move_temp[i])):
								move_temp[i][v] -= center_temp[i]
								move_temp[i][v] = np.dot(rotation_z,move_temp[i][v])

							temp_norm = np.dot(rotation_z,norm_list[i])
							yz_norm = temp_norm[1:]
							yz_norm = yz_norm/la.norm(yz_norm)
							yz_target = np.array([0,-1])
							x_angle = rotation_angle 
							if temp_y > 0:
								x_angle=-x_angle
							else:
								pass

							rev_rotation_z = np.array([[np.cos(-z_angle),-np.sin(-z_angle),0],[np.sin(-z_angle),np.cos(-z_angle),0],[0,0,1]]) 
							rotation_x = np.array([[1,0,0],[0,np.cos(x_angle),-np.sin(x_angle)],[0,np.sin(x_angle),np.cos(x_angle)]]) 
							for v in range(len(move_temp[i])):
								move_temp[i][v] = np.dot(rotation_x,move_temp[i][v])
								move_temp[i][v] = np.dot(rev_rotation_z,move_temp[i][v])						
								move_temp[i][v] += center_temp[i]
							x_rotation_list.append(rotation_x)
							z_rotation_list.append(rotation_z)
							z_rev_rotation_list.append(rev_rotation_z)				
						else:
							x_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
							z_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
							z_rev_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
					else:
						x_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
						z_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))
						z_rev_rotation_list.append(np.array([[1,0,0],[0,1,0],[0,0,1]]))

			for i in range(len(move_temp)-1):
				intersect = polygon_intersection(move_temp[i],move_temp[i+1])
				if intersect:
					done=False
					break
			if done:
				break

		nothing+=1
		if nothing > 15: # 20 -> 10
			print("15번이상 안 변함")
			break
		if good:
			is_change=False
			tooth_temp_list=copy.deepcopy(tooth_list)
			for j in range(len(tooth_temp_list)):
				cog = get_center(tooth_temp_list[j]) 
				move_teeth = center_temp[j]-cog
				for tri in range(len(tooth_temp_list[j].vectors)):
					for i in range(len(tooth_temp_list[j].vectors[tri])):
						tooth_temp_list[j].vectors[tri][i] += move_teeth	
			for j in range(len(tooth_temp_list)):
				for tri in range(len(tooth_temp_list[j].vectors)):
					for i in range(len(tooth_temp_list[j].vectors[tri])):
						tooth_temp_list[j].vectors[tri][i] -= center_temp[j]
						tooth_temp_list[j].vectors[tri][i] = np.dot(z_rotation_list[j],tooth_temp_list[j].vectors[tri][i])
					tooth_temp_list[j].normals[tri] = np.dot(z_rotation_list[j],tooth_temp_list[j].normals[tri])
				for tri in range(len(tooth_temp_list[j].vectors)):	
					for i in range(len(tooth_temp_list[j].vectors[tri])):
						tooth_temp_list[j].vectors[tri][i] = np.dot(x_rotation_list[j],tooth_temp_list[j].vectors[tri][i])
						tooth_temp_list[j].vectors[tri][i] = np.dot(z_rev_rotation_list[j],tooth_temp_list[j].vectors[tri][i])
						tooth_temp_list[j].vectors[tri][i] += center_temp[j]
					tooth_temp_list[j].normals[tri] = np.dot(x_rotation_list[j],tooth_temp_list[j].normals[tri])
					tooth_temp_list[j].normals[tri] = np.dot(z_rev_rotation_list[j],tooth_temp_list[j].normals[tri])
			temp_list_2d=[]
			center_temp=[]
			for i in tooth_temp_list:
				center=get_center(i)
				center_temp.append(center)
				vertex_2d = np.array(get_2d_vertices(i))
				temp_list_2d.append(vertex_2d)

			dist_list=[]
			dir_list=[]
			for i in range(len(tooth_temp_list)):
				dist_dir=direction_to_bone(center_temp[i][:2],bvlist)
				dist_list.append(dist_dir[0])
				dir_list.append(dist_dir[1][:])
			dist_from_bone=0
			for i in dist_list:
				dist_from_bone+=i

			dist_btw_teeth = find_distance_btw_teeth(temp_list_2d)
			dist_btw_teeth_scr = sum(dist_btw_teeth)
			dist_list=[]
			dir_list=[]
			for i in range(len(temp_list_2d)):
				dist_dir=direction_to_bone(center_temp[i][:2],bvlist)
				dist_list.append(dist_dir[0])
				dir_list.append(dist_dir[1][:])
			dist_from_bone_scr = sum(dist_list)

			norm_temp_list=[]
			for i in tooth_temp_list:
				initial_norm=i.normals[-10]
				initial_norm=initial_norm/la.norm(initial_norm)
				norm_temp_list.append(initial_norm)
			diff_angle_scr = 0
			for i in range(len(temp_list_2d)):
				diff_angle_scr += np.arccos(np.dot(correct_angle[i],norm_temp_list[i]))

			teeth_position_scr = 0
			for i in important_teeth:
				teeth_position_scr += la.norm(center_temp[i][:2]-right_position[i])

			current_score = distance_fb_weight*dist_from_bone_scr+distance_bt_weight*dist_btw_teeth_scr+different_ang_weight*diff_angle_scr+position_weight*teeth_position_scr
			result=current_score-best_score
			if result < 0:
				is_change=True
				nothing=0
				improved+=1
				tooth_list=copy.deepcopy(tooth_temp_list)
				moving_center=copy.deepcopy(center_temp)

				best_tooth_list = copy.deepcopy(tooth_temp_list)
				best_score=current_score
				temp_list=copy.deepcopy(move_temp)
				best_list=copy.deepcopy(move_temp)
				best_center=copy.deepcopy(center_temp)
				norm_list=copy.deepcopy(norm_temp_list)

			elif np.random.random() < np.exp(-(result)/temper):
				is_change=True
				tooth_list=copy.deepcopy(tooth_temp_list)
				temp_list=copy.deepcopy(move_temp)
				moving_center=copy.deepcopy(center_temp)
				norm_list=copy.deepcopy(norm_temp_list)
			if is_change:	
				change_count += 1
				if change_count < 60:
					if change_count%6 == 0:
						save_current(change_count,tooth_temp_list)						
				elif change_count%15 == 0:
					save_current(change_count,tooth_temp_list)

			print("current_score : ",current_score,", result : ",result, ", temper : ",temper,", 확률 : ",np.exp(-(result)/temper))
			print(c,"iteration 개별 점수 (dist_from_bone, dist_between_tooth, diff_angle, teeth_position_scr) ",dist_from_bone, dist_btw_teeth_scr, diff_angle_scr, teeth_position_scr)

	print(improved," 개선 후 점수 : ", best_score)

	stop=timeit.default_timer()
	print("runtime : ",stop-start)
	print("수행 결과 best_score : ",best_score)
	print("parameter -- 치아 bone으로부터 score-weight :", distance_fb_weight,", 치아간 점수 : ", distance_bt_weight,", 각도 점수 : ",different_ang_weight,", max_iter",max_iter, ", initial_c = ",initial_c, " , max_temper = ", max_temper, ", nothing bound = 30")
	return best_tooth_list

def save_current(ccount,tooth_temp_list):
	print("중간과정 생성!")
	global test_num
	for i in range(len(tooth_temp_list)):
		tooth_temp_list[i].save('./trajectory/case3/new2/'+test_num+str(ccount)+'_'+str(i+1)+'th_tooth.stl')


def save_change(tlist):
	global test_num
	for i in range(len(tlist)):
		tlist[i].save('./rotation_result/case3/new2/'+test_num+str(i+1)+'.stl')

max_iter=30 
max_temper=20
initial_c=2000
left_right=0.01
to_bone_far=0.01
to_bone_close=0.001
from_bone_far=0.001 
from_bone_close=0.005
rotation_angle=0.015
distance_bt_weight=150
distance_fb_weight=100
different_ang_weight=100
position_weight = 120

to_bone_rotation_prob=0.4
from_bone_rotation_prob=0.7
stay_roatation_prob=0.5

stay_close_prob=0.2
to_bone_close_prob=0.05
from_bone_close_prob=0.05
left_right_close_prob=0.35

stay_far_prob=0.05
to_bone_far_prob=0.3
from_bone_far_prob=0.05
left_right_far_prob=0.3

print("치아 평범이동하고 회전확률 : 30%, 가만히 있을 때 회전 확률 60%, 치아 후진이동하고 회전확률 : 80%")
print("close prob = stay : ",stay_close_prob ,", to_bone : ",to_bone_close_prob,", left_right_prob : ",left_right_close_prob,", from_bone_close_prob: ",from_bone_close_prob)
print("far prob = stay : ",stay_far_prob ,", to_bone : ",to_bone_far_prob,", left_right_prob : ",left_right_far_prob,", from_bone_close_prob: ",from_bone_far_prob)
print("추가로 치아로부터 멀어지는 이동도 추가함 15%확률로 발생")
print("weight (bt, fb, ang, position)",distance_bt_weight,distance_fb_weight,different_ang_weight,position_weight)
print("rotation 확률 : (to_bone, stay, from_bone) ",to_bone_rotation_prob,stay_roatation_prob,from_bone_rotation_prob)
print("rotation angle : ",rotation_angle)
print("---------------------------------")
print("추가적인 사항!!, 양쪽 끝 바깥쪽 방향 (left and right) 으로 가는 거리 (1,1.5,2,2.5)배 random choice로 늘림")
print("case 3")
print("---------------------------------")
test_num="v12_"
print("새 알고리즘", test_num)
tlist=SA_process_3d()
save_change(tlist)
