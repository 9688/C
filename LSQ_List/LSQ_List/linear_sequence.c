#include "linear_sequence.h"
#include <stdlib.h>
#include <string.h>

#define SEQ(T) ((TypeList*)(T))
#define ITR(T) ((TypeIterator*)(T))

typedef struct NodeList{
   struct NodeList *next;
   struct NodeList *prev;
	LSQ_BaseTypeT data;
} TypeListNode;

typedef struct{
	LSQ_IntegerIndexT size;
	TypeListNode *BeforFirst;
	TypeListNode *PastRear;
} TypeList;

typedef struct{
	TypeListNode *node;
	TypeList *list;
}TypeIterator;

/* �������, ��������� ������ ���������. ���������� ����������� ��� ���������� */
LSQ_HandleT LSQ_CreateSequence(void){
	TypeList *list = (TypeList*)malloc(sizeof(TypeList));

	if(list == NULL)
		return(LSQ_HandleInvalid);

	list->BeforFirst = (TypeListNode*)malloc(sizeof(TypeListNode));
	list->PastRear = (TypeListNode*)malloc(sizeof(TypeListNode));
	if(list->BeforFirst == NULL || list->PastRear == NULL){
		free(list->BeforFirst);
		free(list->PastRear);
		free(list);
	}

	list->BeforFirst->prev = NULL;
	list->BeforFirst->next = list->PastRear;
	list->PastRear->prev = list->BeforFirst;
	list->PastRear->next = NULL;
	list->size = 0;
	return(list);
}

/* �������, ������������ ��������� � �������� ������������. ����������� ������������� ��� ������ */
void LSQ_DestroySequence(LSQ_HandleT handle){
	TypeListNode *node = NULL;

	if(handle == NULL)
		return;

	node = SEQ(handle)->BeforFirst;
	while(node->next != NULL){
		node = node->next;
		free(node->prev);
	}
	free(node);
	free(handle);
}


/* �������, ������������ ������� ���������� ��������� � ���������� */
LSQ_IntegerIndexT LSQ_GetSize(LSQ_HandleT handle){
	if(handle == NULL)
		return(0);
	
	return(SEQ(handle)->size);
}

/* �������, ������������, ����� �� ������ �������� ���� ����������� */
int LSQ_IsIteratorDereferencable(LSQ_IteratorT iterator){
	return(iterator == NULL || ITR(iterator)->list == NULL)? 0: (ITR(iterator)->node != ITR(iterator)->list->BeforFirst && ITR(iterator)->node != ITR(iterator)->list->PastRear);
//�������� �� ����� ����������
}

/* �������, ������������, ��������� �� ������ �������� �� �������, ��������� �� ��������� � ���������� */
int LSQ_IsIteratorPastRear(LSQ_IteratorT iterator){
	return(iterator == NULL || ITR(iterator)->list == NULL)? 0:(ITR(iterator)->node == ITR(iterator)->list->PastRear);
//�������� �� ����� ����������
}
/* �������, ������������, ��������� �� ������ �������� �� �������, �������������� ������� � ���������� */
int LSQ_IsIteratorBeforeFirst(LSQ_IteratorT iterator){
	return(iterator == NULL || ITR(iterator)->list == NULL)? 0:(ITR(iterator)->node == ITR(iterator)->list->BeforFirst);
//�������� �� ����� ����������
}


/* ������� ���������������� ��������. ���������� ��������� �� �������, �� ������� ��������� ������ �������� */
LSQ_BaseTypeT* LSQ_DereferenceIterator(LSQ_IteratorT iterator){
	return(!LSQ_IsIteratorDereferencable(iterator))? NULL: (&(ITR(iterator)->node->data));
}
/* ��������� ��� ������� ������� �������� � ������ � ���������� ��� ���������� */
/* �������, ������������ ��������, ����������� �� ������� � ��������� �������� */
LSQ_IteratorT LSQ_GetElementByIndex(LSQ_HandleT handle, LSQ_IntegerIndexT index){
	TypeIterator *iterator = NULL;

	if(handle == NULL)
		return(LSQ_HandleInvalid);

	iterator = (TypeIterator*)malloc(sizeof(TypeIterator));
	iterator->list = SEQ(handle);
	LSQ_SetPosition(iterator, index);
	
	return(iterator);
}
/* �������, ������������ ��������, ����������� �� ������ ������� ���������� */
LSQ_IteratorT LSQ_GetFrontElement(LSQ_HandleT handle){
	return(LSQ_GetElementByIndex(handle, 0));
}
/* �������, ������������ ��������, ����������� �� ��������� ������� ���������� */
LSQ_IteratorT LSQ_GetPastRearElement(LSQ_HandleT handle){
	if(handle == NULL)
		return(LSQ_HandleInvalid);
	return(LSQ_GetElementByIndex(handle, SEQ(handle)->size));
}
/* �������, ������������ �������� � �������� ������������ � ������������� ������������� ��� ������ */
void LSQ_DestroyIterator(LSQ_IteratorT iterator){
	if(iterator != NULL)
		free(iterator);
}

/* �������, ������������ �������� �� ���� ������� ������ */
void LSQ_AdvanceOneElement(LSQ_IteratorT iterator){
	LSQ_ShiftPosition(iterator, 1);
}
/* �������, ������������ �������� �� ���� ������� ����� */
void LSQ_RewindOneElement(LSQ_IteratorT iterator){
	LSQ_ShiftPosition(iterator, -1);
}
/* �������, ������������ �������� �� �������� �������� �� ������ */
void LSQ_ShiftPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT shift){
	if(iterator == NULL)
		return;

	if(shift > 0)
		while(shift > 0 && !LSQ_IsIteratorPastRear(iterator)){
			shift--;
			ITR(iterator)->node = ITR(iterator)->node->next;
		}
	else
		while(shift < 0 && !LSQ_IsIteratorBeforeFirst(iterator)){
			ITR(iterator)->node = ITR(iterator)->node->prev;
			shift++;
		}
}
/* �������, ��������������� �������� �� ������� � ��������� ������� */
void LSQ_SetPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT pos){
	if(iterator == NULL || ITR(iterator)->list == NULL)
		return;
	if(pos <= (ITR(iterator)->list->size / 2)){
		ITR(iterator)->node = ITR(iterator)->list->BeforFirst;
		LSQ_ShiftPosition(iterator, pos + 1);
	}else{
		ITR(iterator)->node = ITR(iterator)->list->PastRear;
		LSQ_ShiftPosition(iterator, pos - ITR(iterator)->list->size);

	}
}
/* �������, ����������� ������� � ������ ���������� */
void LSQ_InsertFrontElement(LSQ_HandleT handle, LSQ_BaseTypeT element){
	TypeIterator *iterator = ITR(LSQ_GetFrontElement(handle));

	if(iterator != LSQ_HandleInvalid){
		LSQ_InsertElementBeforeGiven( iterator, element);
		LSQ_DestroyIterator(iterator);
	}
}
/* �������, ����������� ������� � ����� ���������� */
void LSQ_InsertRearElement(LSQ_HandleT handle, LSQ_BaseTypeT element){
	TypeIterator *iterator = ITR(LSQ_GetPastRearElement(handle));

	if(iterator != LSQ_HandleInvalid){
		LSQ_InsertElementBeforeGiven( iterator, element);
		LSQ_DestroyIterator(iterator);
	}
}

/* �������, ����������� ������� � ��������� �� �������, ����������� � ������ ������ ����������. �������, �� �������  *
 * ��������� ��������, � ����� ��� �����������, ���������� �� ���� ������� � �����.                                  */
void LSQ_InsertElementBeforeGiven(LSQ_IteratorT iterator, LSQ_BaseTypeT newElement){
	TypeListNode *node = NULL;

	if(iterator == NULL)
		return;

	node = (TypeListNode*)malloc(sizeof(TypeListNode));
	if(node == NULL){
		free(node);
		return;
	}
	node->data = newElement;
	node->prev = ITR(iterator)->node->prev;
	node->next = ITR(iterator)->node;
	ITR(iterator)->node->prev->next = node;
	ITR(iterator)->node->prev = node;
	ITR(iterator)->node = ITR(iterator)->node->prev;
	ITR(iterator)->list->size++;
}

/* �������, ��������� ������ ������� ���������� */
void LSQ_DeleteFrontElement(LSQ_HandleT handle){
	TypeIterator *iterator = ITR(LSQ_GetFrontElement(handle));
	if(iterator != LSQ_HandleInvalid){
		LSQ_DeleteGivenElement(iterator);
		LSQ_DestroyIterator(iterator);
	}
}

/* �������, ��������� ��������� ������� ���������� */
void LSQ_DeleteRearElement(LSQ_HandleT handle){
	TypeIterator *iterator = ITR(LSQ_GetPastRearElement(handle));
	if(iterator != LSQ_HandleInvalid){
		iterator->node = iterator->node->prev;
		LSQ_DeleteGivenElement(iterator);
		LSQ_DestroyIterator(iterator);
	}
}
/* �������, ��������� ������� ����������, ����������� �������� ����������. ��� ����������� �������� ��������� ��     *
 * ���� ������� � ������� ������.                                                                                    */
void LSQ_DeleteGivenElement(LSQ_IteratorT iterator){
	TypeListNode *node = NULL;
	if(iterator == NULL || !LSQ_IsIteratorDereferencable(iterator))
		return;
	ITR(iterator)->list->size--;
   ITR(iterator)->node->prev->next = ITR(iterator)->node->next;
   ITR(iterator)->node->next->prev = ITR(iterator)->node->prev;
	node = ITR(iterator)->node;
	ITR(iterator)->node = ITR(iterator)->node->next;
	free(node);
}