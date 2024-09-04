#include <Objects.hpp>


//table functions
table::table(vector<columnInfo> columns, int recs) : recs(recs), columns(columns) {

	//calculate column offsets and stride size
	for (int i = 0; i < columns.size(); i++) {
		this->columns[i].pos = stride + 1;
		stride += columns[i].size;
	}

	//calculate final stride size including flag information
	stride = max(stride, 4) + 1;
	
	//calculate the number of blocks needed to be allocated
	int numBlocks = (int)(ceil(recs / 50.0f));
	//making linked list of allocated memory of the right size and number of nodes
	data = new linkedList(stride * 50, numBlocks);
}

uint8_t* table::getBase(int index) {
	if (index < 0 || index >= recs) return NULL;

	node* blockPtr = data->head;

	while ((index) > 50) {
		blockPtr = blockPtr->next;
		index -= 50;
	}

	uint8_t* base = (uint8_t*)((uint8_t*)blockPtr->data + (index * stride));
	return base;
}

row table::operator[](int index) {
	uint8_t* base = getBase(index);
	return row(base, this, index);
}

table::~table() {
	data->~linkedList();
	columns.~vector();
}

void table::printColumns() {
	int i;
	for (i = 0; i < columns.size() - 1; i++) {
		cout << columns[i].name << ", ";
	}
	cout << columns[i].name << endl;
}

//linked list functions
linkedList::linkedList(size_t dtaSize, int numNodes) {
	head = NULL;
	for (int i = 0; i < numNodes; i++) {
		void* dta = calloc(1, dtaSize);
		node* ptr = new node(dta);
		ptr->next = head;
		head = ptr;
		
		this->numNodes++;
	}
}

linkedList::~linkedList() {
	node* ptr = head;
	while (ptr) {
		node* next = ptr->next;
		delete ptr;
		ptr = next;
	}
}

//node functions
node::~node() {
	free(data);
}

node::node() {}

//row functions
typedValue row::operator[](int index) {
	typedValue val((uint8_t*)(ptr + parent->getColumns()[index].pos), parent->getColumns()[index].type);
	return val;
}

void row::insert(vector<int> indices, vector<typedValue*> values) {
	for (int i = 0; i < indices.size(); i++) {
		int index = indices[i];
		void* addr = this->ptr + parent->getColumns()[index].pos;

		if (parent->getColumns()[index].type != values[i]->type)
			throw exception("Insert type mismatch");

		switch (values[i]->type) {
			case INT: {
				int* ptr = (int*)addr;
				*ptr = *(int*)values[i]->value;
				break;
			}
			case CHAR: {
				char* ptr = (char*)addr;
				*ptr = *(char*)values[i]->value;
				break;
			}
			case VARCHAR: {
				string** ptr = (string**)addr;
				*ptr = (string*)values[i]->value;
				break;
			}
			case FLOAT: {
				float* ptr = (float*)addr;
				*ptr = *(float*)values[i]->value;
				break;
			}
		}
	}
}

void row::print() {
	int i;
	for (i = 0; i < parent->getColumns().size() - 1; i++) {
		cout << operator[](i).getStringRep() << ',';
	}
	cout << operator[](i).getStringRep() << endl;
}

row row::operator++(int) {
	row old = *this;
	index++;
	ptr = parent->getBase(index);
	return old;
}

row row::operator--(int) {
	row old = *this;
	index--;
	ptr = parent->getBase(index);
	return old;
}

row::operator bool() {
	return ptr != NULL;
}

