#include "DCEL_IO.h"


//using namespace DCEL_IO;

DCEL DCEL_IO::text_to_DCEL(std::string fname) {
	DCEL ret;
	std::ifstream fp;
	fp.open(fname);
	if(!fp.is_open()) return ret;

	std::string buf;
	std::getline(fp, buf);
	std::stringstream stream(buf);
	int n_v, n_f, n_e;
	stream >>n_v >>n_f >> n_e;

	if(__DEBUG_MODE__)
		std::cout <<n_v<<' '<< n_f<<' '<<n_e <<std::endl; 

	std::vector<Vertex*> vertices(n_v);
	for(int i = 0 ; i< n_v;i++) vertices[i] = new Vertex();
	std::vector<Face*> faces(n_f);
	for(int i = 0 ; i< n_f;i++) faces[i] = new Face();
	std::vector<HEdge*> hedges(n_e);
	for(int i = 0 ; i< n_e;i++) hedges[i] = new HEdge();

	//Set key of elements
	std::getline(fp, buf);
	stream = std::stringstream(buf);
	for(int i = 0 ; i<n_v;i++){
		std::string key;
		stream >> key ;
		vertices[i]->setKey(key);

		if(__DEBUG_MODE__) std::cout << key <<' '; 
	}

	if(__DEBUG_MODE__) std::cout <<std::endl; 

	std::getline(fp, buf);
	stream = std::stringstream(buf);
	for(int i = 0 ; i<n_f;i++){
		std::string key;
		stream >> key ;
		faces[i]->setKey(key);

		if(__DEBUG_MODE__) std::cout << key <<' '; 
	}

	if(__DEBUG_MODE__) std::cout <<std::endl; 

	std::getline(fp, buf);
	stream = std::stringstream(buf);
	for(int i = 0 ; i<n_e;i++){
		std::string key;
		stream >> key ;
		hedges[i]->setKey(key);

		if(__DEBUG_MODE__) std::cout << key <<' '; 
	}
	if(__DEBUG_MODE__) std::cout <<std::endl; 

	ret.setVertices(vertices);
	ret.setFaces(faces);
	ret.setHedges(hedges);

	//Set vertex info
	for(int i = 0 ;i<n_v ;i++){
		std::getline(fp, buf);
		stream = std::stringstream(buf);
		std::string vertex_key;
		double x,y;
		std::string hedge_key;
		stream >> vertex_key >> x >> y >> hedge_key;
		Vertex* v_ptr = ret.getVertex(vertex_key);
		v_ptr->setx(x);
		v_ptr->sety(y);
		HEdge* he_ptr = ret.getHedge(hedge_key);
		v_ptr->setIncidentEdge(he_ptr);

		if(__DEBUG_MODE__) std::cout << vertex_key <<' '<<x <<' '<< y <<' '<< hedge_key<<std::endl; 
	}

	//Set Face info
	for(int i = 0 ; i<n_f;i++){
		std::getline(fp, buf);
		stream = std::stringstream(buf);
		std::string face_key;
		int is_out;
		std::string outer_key;
		int num_inner;
		stream >> face_key >> is_out >> outer_key >> num_inner;
		Face *f_ptr=ret.getFace(face_key);
		HEdge *e_ptr = ret.getHedge(outer_key);

		if(!is_out) f_ptr->setOuter(e_ptr);

		if(__DEBUG_MODE__) std::cout << face_key <<' '<<is_out <<' '<< outer_key << ' '<<num_inner<< ' ';

		for(int j = 0 ; j< num_inner;j++){
			std::string inner_e_key;
			stream >> inner_e_key;
			HEdge *e_ptr = ret.getHedge(inner_e_key); 
			f_ptr->addInner(e_ptr);

			if(__DEBUG_MODE__) std::cout <<inner_e_key << ' ';
		}
		if(__DEBUG_MODE__) std::cout <<std::endl; 
	}

	//Set HEdge info
	for(int i = 0; i<n_e ; i++){
		std::getline(fp, buf);
		stream = std::stringstream(buf);
		std::string hedge_key, origin_key, twin_key, incident_f_key, next_key, prev_key;

		stream >> hedge_key >> origin_key >> twin_key >> incident_f_key >> next_key >> prev_key;

		HEdge* he_ptr = ret.getHedge(hedge_key);
		Vertex *origin_ptr = ret.getVertex(origin_key);
		HEdge *twin_ptr = ret.getHedge(twin_key);
		Face *incident_f_ptr = ret.getFace(incident_f_key);
		HEdge *next_ptr = ret.getHedge(next_key);
		HEdge *prev_ptr = ret.getHedge(prev_key);
		he_ptr->setOrigin(origin_ptr);
		he_ptr->setTwin(twin_ptr);
		he_ptr->setIncidentFace(incident_f_ptr);
		he_ptr->setNext(next_ptr);
		he_ptr->setPrev(prev_ptr);

		if(__DEBUG_MODE__) std::cout << hedge_key <<' '<<origin_key <<' '<< twin_key << ' '<<incident_f_key<< ' '<<next_key<<' '<<prev_key<<std::endl;
	}
	fp.close();

	return ret;
}

void DCEL_IO::print_DCEL(DCEL &dcel){
	std::vector<Vertex*> vertices = dcel.getVertices();
	std::vector<HEdge*> hedges = dcel.getHedges();
	std::vector<Face*> faces = dcel.getFaces();

	std::cout << "|V|= "<<vertices.size() << "\t|E|= "<<hedges.size() << "\t|F|= "<<faces.size() << std::endl;
	std::cout << "<Vertices>"<<std::endl;
	for(auto v:vertices){
		std::cout <<v->getKey()<<"\t"<< v->getPoint() <<"\t"<< v->getIncidentEdge()->getKey()<<std::endl;
	}
	std::cout << "<Faces>"<<std::endl;
	for(auto f:faces){
		std::cout <<f->getKey() << "\t" << (f->isOutMost() ? "UBD":"BD\t"+f->getOuter()->getKey()) << "\t";
		if(f->getInners().size() > 0) {
			std::cout <<"Inners: ";
			for(auto he:f->getInners())
				std::cout <<he->getKey() <<"\t";
		}
		std::cout <<std::endl;
	}
	std::cout << "<Edges>"<<std::endl;
	for(auto e:hedges){
		std::cout << e->getKey() <<"\t"<<e->getOrigin()->getKey()<<"\t"<<e->getTwin()->getKey()<<'\t';
		std::cout<< e->getIncidentFace()->getKey() <<'\t'<< e->getNext()->getKey() <<'\t'<< e->getPrev()->getKey()<<std::endl;
	}
}

/*
void DCEL_IO::DCELtotext(FILE* readFile) {
	char* buffer = new char[BUFFERSIZE];
	sprintf(buffer, "%d\t%d\t%d\n", this->vertices->size(), this->faces->size(), this->hedges->size());
	fputs(buffer, readFile);

	int i = 0;
	for (i = 0; i < this->vertices->size() - 1; i++) {
		sprintf(buffer, "%s\t", (*this->vertices)[i]->getVertexKey());
		fputs(buffer, readFile);
	}
	if (i == this->vertices->size() - 1) {
		sprintf(buffer, "%s\n", (*this->vertices)[i]->getVertexKey());
		fputs(buffer, readFile);
	}

	for (i = 0; i < this->faces->size() - 1; i++) {
		sprintf(buffer, "%s\t", (*this->faces)[i]->getFaceKey());
		fputs(buffer, readFile);
	}
	if (i == this->faces->size() - 1) {
		sprintf(buffer, "%s\n", (*this->faces)[i]->getFaceKey());
		fputs(buffer, readFile);
	}

	for (i = 0; i < this->hedges->size() - 1; i++) {
		sprintf(buffer, "%s\t", (*this->hedges)[i]->getHedgeKey());
		fputs(buffer, readFile);
	}
	if (i == this->hedges->size() - 1) {
		sprintf(buffer, "%s\n", (*this->hedges)[i]->getHedgeKey());
		fputs(buffer, readFile);
	}
	for (i = 0; i < this->vertices->size(); i++)
	{
		sprintf(buffer, "%s\t%.3lf,%.3lf\t%s\n", (*this->vertices)[i]->getVertexKey(), (*this->vertices)[i]->getx(), (*this->vertices)[i]->gety(), (*this->vertices)[i]->getIncidentEdge()->getHedgeKey());
		fputs(buffer, readFile);
	}
	for (i = 0; i < this->faces->size(); i++)
	{
		sprintf(buffer, "%s\t", (*this->faces)[i]->getFaceKey());
		fputs(buffer, readFile);

		if ((*this->faces)[i]->getOuter() == nullptr) {
			sprintf(buffer, "NULL\t%d\t", (*this->faces)[i]->getInners()->size());
			fputs(buffer, readFile);
		}
		else {
			sprintf(buffer, "%s\t%d\t", (*this->faces)[i]->getOuter()->getHedgeKey(), (*this->faces)[i]->getInners()->size());
			fputs(buffer, readFile);
		}

		int j = 0;
		if ((*this->faces)[i]->getInners()->size() == 0) {
			fputs("NULL\n", readFile);
		}

		else {
			for (j = 0; j < (*this->faces)[i]->getInners()->size() - 1; j++) {
				sprintf(buffer, "%s\t", (*(*this->faces)[i]->getInners())[j]->getHedgeKey());
				fputs(buffer, readFile);
			}

			if (j == (*this->faces)[i]->getInners()->size() - 1) {
				sprintf(buffer, "%s\n", (*(*this->faces)[i]->getInners())[j]->getHedgeKey());
				fputs(buffer, readFile);
			}
		}
	}

	for (i = 0; i < this->hedges->size() - 1; i++) {
		sprintf(buffer, "%s\t%s\t%s\t%s\t%s\t%s\n", (*this->hedges)[i]->getHedgeKey(), (*this->hedges)[i]->getOrigin()->getVertexKey(), (*this->hedges)[i]->getTwin()->getHedgeKey(), (*this->hedges)[i]->getIncidentFace()->getFaceKey(), (*this->hedges)[i]->getNext()->getHedgeKey(), (*this->hedges)[i]->getPrev()->getHedgeKey());
		fputs(buffer, readFile);
	}
	if (i == this->hedges->size() - 1) {
		sprintf(buffer, "%s\t%s\t%s\t%s\t%s\t%s", (*this->hedges)[i]->getHedgeKey(), (*this->hedges)[i]->getOrigin()->getVertexKey(), (*this->hedges)[i]->getTwin()->getHedgeKey(), (*this->hedges)[i]->getIncidentFace()->getFaceKey(), (*this->hedges)[i]->getNext()->getHedgeKey(), (*this->hedges)[i]->getPrev()->getHedgeKey());
		fputs(buffer, readFile);
	}
}

void DCEL_IO::printVertexTab() {
	std::cout << "\n" << "*********** Vertex Table ************" << "\n";
	std::cout << "vertex" << "\tCoordinates " << "\tIncident Edge " << "\n";

	for (int i = 0; i < this->vertices->size(); i++)
	{
		std::cout << std::setw(4) << (*this->vertices)[i]->getVertexKey() << std::setw(6) << "(" << std::setw(2) << (*this->vertices)[i]->getx() << ", " << std::setw(2) << (*this->vertices)[i]->gety() << ")" << std::setw(14) << (*this->vertices)[i]->getIncidentEdge()->getHedgeKey() << std::endl;
	}
}
void DCEL_IO::printHedgeTab() {
	std::cout << "\n" << "****************** Half-edge Table ******************" << "\n";
	std::cout << "Half-edge " << " Origin " << "  Twin" << "  IncidentFace" << "  Next" << "    Prev" << "\n";
	for (int i = 0; i < this->hedges->size(); i++)
	{
		HEdge* he = (*this->hedges)[i];
		std::cout << std::setw(7) << he->getHedgeKey() << std::setw(8) << he->getOrigin()->getVertexKey() << std::setw(9) << he->getTwin()->getHedgeKey() << "\t" << he->getIncidentFace()->getFaceKey() << "\t" << he->getNext()->getHedgeKey() << "\t" << he->getPrev()->getHedgeKey() << std::endl;
		he = he->getTwin();
		std::cout << std::setw(7) << he->getHedgeKey() << std::setw(8) << he->getOrigin()->getVertexKey() << std::setw(9) << he->getTwin()->getHedgeKey() << "\t" << he->getIncidentFace()->getFaceKey() << "\t" << he->getNext()->getHedgeKey() << "\t" << he->getPrev()->getHedgeKey() << std::endl;
	}
}

void DCEL_IO::printFaceTab() {
	std::cout << "\n" << "************ Face Table *************" << "\n";
	std::cout << "Face " << " OuterComponent " << "InnerComponents" << "\n";
	for (int i = 0; i < this->faces->size(); i++)
	{
		std::cout << std::setw(4) << (*this->faces)[i]->getFaceKey() << "\t";

		if ((*this->faces)[i]->getOuter() == nullptr) {
			std::cout << " NULL \t\t";
		}
		else {
			std::cout << " " << (*this->faces)[i]->getOuter()->getHedgeKey() << "\t\t";
		}

		if (((*this->faces)[i]->getInners()->size() == 0)) {
			std::cout << "  NULL \t";

		}
		else {
			std::cout << "  ";
			for (int j = 0; j < (*this->faces)[i]->getInners()->size(); j++) {
				std::cout << (*(*this->faces)[i]->getInners())[j]->getHedgeKey() << "  ";
			}
		}
		std::cout << std::endl;
	}
}*/
