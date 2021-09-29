#include "DCEL_IO.h"
#include <cstring>
#include <iostream>
#include <iomanip>

#define BUFFERSIZE 1000
DCEL_IO::DCEL_IO(FILE* readFile) {
	char* buffer = new char[BUFFERSIZE];
	fgets(buffer, BUFFERSIZE, readFile);

	// Number of Faces HEdges and Vertex
	char* token = strtok(buffer, "\t, \n");
	this->num_vertices = atoi(token);

	token = strtok(NULL, "\t, \n");
	this->num_faces = atoi(token);
	
	token = strtok(NULL, "\t, \n");
	this->num_hedges = atoi(token);
	
	this->vertices = new std::vector<Vertex*>(num_vertices);
	for (int i = 0; i < num_vertices; i++) {
		(*this->vertices)[i] = new Vertex();
	}

	this->faces = new std::vector<Face*>(num_faces);
	for (int i = 0; i < num_faces; i++) {
		(*this->faces)[i] = new Face();
	}
	this->hedges = new std::vector<HEdge*>(num_hedges/2);
	for (int i = 0; i < num_hedges/2; i++) {
		(*this->hedges)[i] = new HEdge();
		HEdge *twin = new HEdge();
		(*this->hedges)[i]->setTwin(twin);
		twin->setTwin((*this->hedges)[i]);
	}

	//Set Keys of Faces, HEdges, and Vertices
	fgets(buffer, BUFFERSIZE, readFile);
	token = strtok(buffer, "\t, \n");
	(*this->vertices)[0]->setVertexKey(token);

	for (int i = 1; i < this->num_vertices; i++) {
		token = strtok(NULL, "\t, \n");
		(*this->vertices)[i]->setVertexKey(token);
	}


	fgets(buffer, BUFFERSIZE, readFile);
	token = strtok(buffer, "\t, \n");
	(*this->faces)[0]->setFaceKey(token);

	for (int i = 1; i < this->num_faces; i++) {
		token = strtok(NULL, "\t, \n");
		(*this->faces)[i]->setFaceKey(token);
	}

	fgets(buffer, BUFFERSIZE, readFile);

	token = strtok(buffer, "\t, \n");
	(*this->hedges)[0]->setHedgeKey(token);
	token = strtok(NULL, "\t, \n");
	(*this->hedges)[0]->getTwin()->setHedgeKey(token);

	for (int i = 1; i < this->num_hedges/2; i++) {
		token = strtok(NULL, "\t, \n");
		(*this->hedges)[i]->setHedgeKey(token);
		token = strtok(NULL, "\t, \n");
		(*this->hedges)[i]->getTwin()->setHedgeKey(token);
	}

	// Set Vertex information
	for (int i = 0; i < num_vertices; i++) {
		fgets(buffer, BUFFERSIZE, readFile);
		token = strtok(buffer, "\t, \n");
		token = strtok(NULL, "\t, \n");
		(*this->vertices)[i]->setx(atof(token));
		token = strtok(NULL, "\t, \n");
		(*this->vertices)[i]->sety(atof(token));
		token = strtok(NULL, "\t, \n");
		(*this->vertices)[i]->setIncidentEdge(this->searchHedge(token));

		if (this->lmost)  this->lmost = this->lmost->getx() > (*this->vertices)[i]->getx() ? (*this->vertices)[i] : this->lmost;
		else this->lmost = (*this->vertices)[i];
		if (this->rmost)  this->rmost = this->rmost->getx() < (*this->vertices)[i]->getx() ? (*this->vertices)[i] : this->rmost;
		else this->rmost = (*this->vertices)[i];
		if (this->bmost)  this->bmost = this->bmost->gety() > (*this->vertices)[i]->gety() ? (*this->vertices)[i] : this->bmost;
		else this->bmost = (*this->vertices)[i];
		if (this->tmost)  this->tmost = this->tmost->gety() < (*this->vertices)[i]->gety() ? (*this->vertices)[i] : this->tmost;
		else this->tmost = (*this->vertices)[i];
	}

	// Set Face information
	int num_inner_components = 0;
	for (int i = 0; i < num_faces; i++) {
		fgets(buffer, BUFFERSIZE, readFile);
		token = strtok(buffer, "\t, \n");
		token = strtok(NULL, "\t, \n");
		(*this->faces)[i]->setOuter(this->searchHedge(token));
		token = strtok(NULL, "\t, \n");
		num_inner_components = atoi(token);

		for (int j = 0; j < num_inner_components; j++) {
			token = strtok(NULL, "\t, \n");
			(*this->faces)[i]->getInners()->push_back(this->searchHedge(token));
		}
	}

	// Set Half Edge information
	for (int i = 0; i < num_hedges; i++) {
		HEdge *he;
		if(i %2 == 0) he = (*this->hedges)[i/2];
		else he = (*this->hedges)[i/2]->getTwin();
		fgets(buffer, BUFFERSIZE, readFile);
		token = strtok(buffer, "\t, \n");
		token = strtok(NULL, "\t, \n");
		he->setOrigin(this->searchVertex(token));
		token = strtok(NULL, "\t, \n");
		he->setTwin(this->searchHedge(token));
		token = strtok(NULL, "\t, \n");
		he->setIncidentFace(this->searchFace(token));
		token = strtok(NULL, "\t, \n");
		he->setNext(this->searchHedge(token));
		token = strtok(NULL, "\t, \n");
		he->setPrev(this->searchHedge(token));
	}

	// Set Edge information of HEdges
	for (int i = 0; i < num_hedges/2; i++) {
		(*this->hedges)[i]->sets((*this->hedges)[i]->getOrigin());
		(*this->hedges)[i]->sett((*this->hedges)[i]->getTwin()->getOrigin());
		(*this->hedges)[i]->getTwin()->sets((*this->hedges)[i]->getTwin()->getOrigin());
		(*this->hedges)[i]->getTwin()->sett((*this->hedges)[i]->getOrigin());
	}
}


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
}
