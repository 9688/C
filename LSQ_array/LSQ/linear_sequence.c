#include"linear_sequence.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SEQ(T) ((LSQ_TypeSequence*)(T))
#define ITR(T) ((LSQ_TypeIterator*)(T))

typedef struct {
	LSQ_BaseTypeT *head;
	LSQ_IntegerIndexT size;
} LSQ_TypeSequence;

typedef struct{
	LSQ_IntegerIndexT index;
	LSQ_TypeSequence *SeqHandle;
} LSQ_TypeIterator;

/* �������, ��������� ������ ���������. ���������� ����������� ��� ���������� */
LSQ_HandleT LSQ_CreateSequence(){
	LSQ_TypeSequence *sequence = NULL;
	sequence = (LSQ_TypeSequence*)malloc(sizeof(LSQ_TypeSequence));
	sequence->size = 0;
	sequence->head = NULL;
	return ((LSQ_HandleT)sequence);
}

/* �y�����, ������������ ��������� � �������� ������������. ����������� ������������� ��� ������ */
void LSQ_DestroySequence(LSQ_HandleT handle){
	if (handle == LSQ_HandleInvalid) return;
	free(SEQ(handle)->head);
	free(SEQ(handle));
}

/* �������, ������������ ������� ���������� ��������� � ���������� */
LSQ_IntegerIndexT LSQ_GetSize(LSQ_HandleT handle){
	if (handle == LSQ_HandleInvalid) return (0);
	return (SEQ(handle)->size);
}

/* �������, ������������, ����� �� ������ �������� ���� ����������� */
int LSQ_IsIteratorDereferencable(LSQ_IteratorT iterator){
	return (iterator != NULL && ITR(iterator)->index >= 0 && ITR(iterator)->index <= ITR(iterator)->SeqHandle->size - 1);
}

/* �������, ������������, ��������� �� ������ �������� �� �������, ��������� �� ��������� � ���������� */
int LSQ_IsIteratorPastRear(LSQ_IteratorT iterator){
	return((iterator != NULL && ITR(iterator)->index >= ITR(iterator)->SeqHandle->size));
}

/* �������, ������������, ��������� �� ������ �������� �� �������, �������������� ������� � ���������� */
int LSQ_IsIteratorBeforeFirst(LSQ_IteratorT iterator){
	return(iterator != NULL && ITR(iterator)->index < 0);
}

/* ������� ���������������� ��������. ���������� ��������� �� �������, �� ������� ��������� ������ �������� */
LSQ_BaseTypeT* LSQ_DereferenceIterator(LSQ_IteratorT iterator){
	return (iterator==NULL)? NULL : (ITR(iterator)->SeqHandle->head + ITR(iterator)->index);
}

/* �������, ������������ ��������, ����������� �� ������� � ��������� �������� */
LSQ_IteratorT LSQ_GetElementByIndex(LSQ_HandleT handle, LSQ_IntegerIndexT index){
	LSQ_TypeIterator *iterator = NULL;
	
	if (handle == LSQ_HandleInvalid) return (NULL);
	iterator = (LSQ_TypeIterator*)malloc(sizeof(LSQ_TypeIterator));
   iterator->index = index;
	iterator->SeqHandle = SEQ(handle);
	return ((LSQ_IteratorT)iterator);
}

/* �������, ������������ ��������, ����������� �� ������ ������� ���������� */
LSQ_IteratorT LSQ_GetFrontElement(LSQ_HandleT handle){
	LSQ_TypeIterator *iterator = NULL;
	if (handle == LSQ_HandleInvalid) return (LSQ_HandleInvalid);
	iterator = (LSQ_TypeIterator*)malloc(sizeof(LSQ_TypeIterator));
	iterator->index = 0;
	iterator->SeqHandle = SEQ(handle);
	return ((LSQ_IteratorT)iterator);
}

/* �������, ������������ ��������, ����������� �� �������� ����� ���������� ������� ���������� */
LSQ_IteratorT LSQ_GetPastRearElement(LSQ_HandleT handle){
	LSQ_TypeIterator *iterator = NULL;
	if (SEQ(handle) == LSQ_HandleInvalid) return (LSQ_HandleInvalid);
	iterator = (LSQ_TypeIterator*)malloc(sizeof(LSQ_TypeIterator));
	iterator->index = SEQ(handle)->size;
	iterator->SeqHandle = SEQ(handle);
	iterator->index = SEQ(handle)->size;
	return ((LSQ_IteratorT)iterator);
}

/* �������, ������������ �������� � �������� ������������ � ������������� ������������� ��� ������ */
void LSQ_DestroyIterator(LSQ_IteratorT iterator){
    free(ITR(iterator));
}

/* �������, ������������ �������� �� ���� ������� ������ */
void LSQ_AdvanceOneElement(LSQ_IteratorT iterator){
	if (ITR(iterator) == NULL) return;
	ITR(iterator)->index++;
}

/* �������, ������������ �������� �� ���� ������� ����� */
void LSQ_RewindOneElement(LSQ_IteratorT iterator){
	if (ITR(iterator) == NULL) return;
	ITR(iterator)->index--;
}

/* �������, ������������ �������� �� �������� �������� �� ������ */
void LSQ_ShiftPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT shift){
	if (ITR(iterator) == NULL) return;
	ITR(iterator)->index += shift;
}

/* �������, ��������������� �������� �� ������� � ��������� ������� */
void LSQ_SetPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT pos){
	if (ITR(iterator) == NULL) return;
	ITR(iterator)->index = pos;
}

/* �������, ����������� ������� � ������ ���������� */
void LSQ_InsertFrontElement(LSQ_HandleT handle, LSQ_BaseTypeT element){
	if (SEQ(handle) == LSQ_HandleInvalid) return;
	SEQ(handle)->size++; 
	SEQ(handle)->head = (LSQ_BaseTypeT*)realloc(SEQ(handle)->head, sizeof(LSQ_BaseTypeT)*(SEQ(handle)->size));
	memmove(SEQ(handle)->head + 1, SEQ(handle)->head, sizeof(LSQ_BaseTypeT)*(SEQ(handle)->size - 1));
	*(SEQ(handle)->head) = element;
}

/* �������, ����������� ������� � ����� ���������� */
void LSQ_InsertRearElement(LSQ_HandleT handle, LSQ_BaseTypeT element){
	if (SEQ(handle) == LSQ_HandleInvalid) return; 
	SEQ(handle)->size = SEQ(handle)->size + 1;
	SEQ(handle)->head = (LSQ_BaseTypeT*)realloc(SEQ(handle)->head, sizeof(LSQ_BaseTypeT)*(SEQ(handle)->size));
   *(SEQ(handle)->head + SEQ(handle)->size - 1) = element;
}

/* �������, ����������� ������� � ��������� �� �������, ����������� � ������ ������ ����������. �������, �� �������  *
 * ��������� ��������, � ����� ��� �����������, ���������� �� ���� ������� � �����.                                  */
void LSQ_InsertElementBeforeGiven(LSQ_IteratorT iterator, LSQ_BaseTypeT newElement){
	if (ITR(iterator) == NULL) return;
	ITR(iterator)->SeqHandle->size++;
	
	ITR(iterator)->SeqHandle->head = (LSQ_BaseTypeT*)realloc(ITR(iterator)->SeqHandle->head, sizeof(LSQ_BaseTypeT)*(ITR(iterator)->SeqHandle->size));
	memmove(ITR(iterator)->SeqHandle->head + ITR(iterator)->index + 1, ITR(iterator)->SeqHandle->head + ITR(iterator)->index,sizeof(LSQ_BaseTypeT)*(ITR(iterator)->SeqHandle->size - ITR(iterator)->index - 1));
	*(ITR(iterator)->SeqHandle->head + ITR(iterator)->index) = newElement;
}

/* �������, ��������� ������ ������� ���������� */
void LSQ_DeleteFrontElement(LSQ_HandleT handle){
	if (SEQ(handle) == LSQ_HandleInvalid || SEQ(handle)->size == 0) return;
	SEQ(handle)->size--;
	memmove(SEQ(handle)->head, SEQ(handle)->head + 1, sizeof(LSQ_BaseTypeT)*(SEQ(handle)->size));
	SEQ(handle)->head = (LSQ_BaseTypeT*)realloc(SEQ(handle)->head, sizeof(LSQ_BaseTypeT)*(SEQ(handle)->size));
}

/* �������, ��������� ��������� ������� ���������� */
void LSQ_DeleteRearElement(LSQ_HandleT handle){
	if (SEQ(handle) == LSQ_HandleInvalid || SEQ(handle)->size == 0) return;
	SEQ(handle)->size--;
	SEQ(handle)->head = (LSQ_BaseTypeT*)realloc(SEQ(handle)->head, sizeof(LSQ_BaseTypeT)*(SEQ(handle)->size));
}

/* �������, ��������� ������� ����������, ����������� �������� ����������. ��� ����������� �������� ��������� ��     *
 * ���� ������� � ������� ������.                                                                                    */
void LSQ_DeleteGivenElement(LSQ_IteratorT iterator){
	if (ITR(iterator) == NULL || !LSQ_IsIteratorDereferencable(iterator)) return;
	ITR(iterator)->SeqHandle->size--;
	memmove(ITR(iterator)->SeqHandle->head + ITR(iterator)->index , ITR(iterator)->SeqHandle->head + ITR(iterator)->index + 1, sizeof(LSQ_BaseTypeT)*(ITR(iterator)->SeqHandle->size - ITR(iterator)->index));
	ITR(iterator)->SeqHandle->head = (LSQ_BaseTypeT*)realloc(ITR(iterator)->SeqHandle->head,sizeof(LSQ_BaseTypeT)*(ITR(iterator)->SeqHandle->size));
}