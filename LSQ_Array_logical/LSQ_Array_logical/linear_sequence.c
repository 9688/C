#include"linear_sequence.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SEQ(T) ((TypeSequence*)(T))
#define ITR(T) ((TypeIterator*)(T))

typedef struct {
	LSQ_BaseTypeT *head;
	LSQ_IntegerIndexT logSize,FizSize;
} TypeSequence;

typedef struct{
	LSQ_IntegerIndexT index;
	TypeSequence *SeqHandle;
} TypeIterator;

/* �������, ��������� ������ ���������. ���������� ����������� ��� ���������� */
LSQ_HandleT LSQ_CreateSequence(){
	TypeSequence *sequence = NULL;
	sequence = (TypeSequence*)malloc(sizeof(TypeSequence));
	sequence->logSize = 0; 
	sequence->FizSize = 4;
	sequence->head = (LSQ_BaseTypeT*)malloc(sizeof(LSQ_BaseTypeT) * sequence->FizSize);
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
	return (SEQ(handle)->logSize);
}

/* �������, ������������, ����� �� ������ �������� ���� ����������� */
int LSQ_IsIteratorDereferencable(LSQ_IteratorT iterator){
	return (!LSQ_IsIteratorPastRear(iterator) && !LSQ_IsIteratorBeforeFirst(iterator) && iterator != NULL);
}

/* �������, ������������, ��������� �� ������ �������� �� �������, ��������� �� ��������� � ���������� */
int LSQ_IsIteratorPastRear(LSQ_IteratorT iterator){
	return((iterator != NULL && ITR(iterator)->index >= ITR(iterator)->SeqHandle->logSize));
}

/* �������, ������������, ��������� �� ������ �������� �� �������, �������������� ������� � ���������� */
int LSQ_IsIteratorBeforeFirst(LSQ_IteratorT iterator){
	return(iterator != NULL && ITR(iterator)->index < 0);
}

/* ������� ���������������� ��������. ���������� ��������� �� �������, �� ������� ��������� ������ �������� */
LSQ_BaseTypeT* LSQ_DereferenceIterator(LSQ_IteratorT iterator){
	return (!LSQ_IsIteratorDereferencable(iterator))? NULL : (ITR(iterator)->SeqHandle->head + ITR(iterator)->index);
}

/* �������, ������������ ��������, ����������� �� ������� � ��������� �������� */
LSQ_IteratorT LSQ_GetElementByIndex(LSQ_HandleT handle, LSQ_IntegerIndexT index){
	TypeIterator *iterator = NULL;
	
	if (handle == LSQ_HandleInvalid) return (NULL);
	iterator = (TypeIterator*)malloc(sizeof(TypeIterator));
    iterator->index = index;
	iterator->SeqHandle = SEQ(handle);
	return ((LSQ_IteratorT)iterator);
}

/* �������, ������������ ��������, ����������� �� ������ ������� ���������� */
LSQ_IteratorT LSQ_GetFrontElement(LSQ_HandleT handle){
	return(LSQ_GetElementByIndex(handle, 0));
}

/* �������, ������������ ��������, ����������� �� �������� ����� ���������� ������� ���������� */
LSQ_IteratorT LSQ_GetPastRearElement(LSQ_HandleT handle){
	return(handle == NULL)? NULL: (LSQ_GetElementByIndex(handle,SEQ(handle)->logSize));
}

/* �������, ������������ �������� � �������� ������������ � ������������� ������������� ��� ������ */
void LSQ_DestroyIterator(LSQ_IteratorT iterator){
    free(iterator);
}

/* �������, ������������ �������� �� ���� ������� ������ */
void LSQ_AdvanceOneElement(LSQ_IteratorT iterator){
	if (ITR(iterator) == NULL)
		return;
	ITR(iterator)->index++;
}

/* �������, ������������ �������� �� ���� ������� ����� */
void LSQ_RewindOneElement(LSQ_IteratorT iterator){
	if (ITR(iterator) == NULL) 
		return;
	ITR(iterator)->index--;
}

/* �������, ������������ �������� �� �������� �������� �� ������ */
void LSQ_ShiftPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT shift){
	if (ITR(iterator) == NULL)
		return;
	ITR(iterator)->index += shift;
}

/* �������, ��������������� �������� �� ������� � ��������� ������� */
void LSQ_SetPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT pos){
	if (ITR(iterator) == NULL)
		return;
	ITR(iterator)->index = pos;
}

/* �������, ����������� ������� � ������ ���������� */
void LSQ_InsertFrontElement(LSQ_HandleT handle, LSQ_BaseTypeT element){
	TypeIterator *iterator = ITR(LSQ_GetElementByIndex(handle, 0));

	if (iterator == NULL)
		return;
	LSQ_InsertElementBeforeGiven(iterator, element);
	LSQ_DestroyIterator(iterator);
}

/* �������, ����������� ������� � ����� ���������� */
void LSQ_InsertRearElement(LSQ_HandleT handle, LSQ_BaseTypeT element){
	TypeIterator *iterator = NULL;
	if (handle == NULL)
		return;
	iterator = ITR(LSQ_GetElementByIndex(handle, SEQ(handle)->logSize));
	if (iterator == NULL)
		return;
	LSQ_InsertElementBeforeGiven(iterator, element);
	LSQ_DestroyIterator(iterator);
}

/*�������, �������������� ���������� ������ ����������*/
void ResizeSequense(LSQ_HandleT handle, int resize){
	if(resize == 1 && SEQ(handle)->logSize == SEQ(handle)->FizSize){
		SEQ(handle)->head = (LSQ_BaseTypeT*)realloc(SEQ(handle)->head,sizeof(LSQ_BaseTypeT)*(SEQ(handle)->logSize)*2);
		SEQ(handle)->FizSize *= 2;
	}else
		if(resize == -1 && SEQ(handle)->logSize == SEQ(handle)->FizSize - SEQ(handle)->FizSize % 4){
			SEQ(handle)->head = (LSQ_BaseTypeT*)realloc(SEQ(handle)->head,sizeof(LSQ_BaseTypeT)*(SEQ(handle)->FizSize - SEQ(handle)->FizSize % 4));
			SEQ(handle)->FizSize = SEQ(handle)->FizSize - SEQ(handle)->FizSize % 4;
		}
}

/* �������, ����������� ������� � ��������� �� �������, ����������� � ������ ������ ����������. �������, �� �������  *
 * ��������� ��������, � ����� ��� �����������, ���������� �� ���� ������� � �����.                                  */
void LSQ_InsertElementBeforeGiven(LSQ_IteratorT iterator, LSQ_BaseTypeT newElement){
	if (ITR(iterator) == NULL || ITR(iterator)->SeqHandle == NULL)
		return;
	ResizeSequense(ITR(iterator)->SeqHandle,1);
	ITR(iterator)->SeqHandle->logSize++;
	memmove(ITR(iterator)->SeqHandle->head + ITR(iterator)->index + 1, ITR(iterator)->SeqHandle->head + ITR(iterator)->index,sizeof(LSQ_BaseTypeT)*(ITR(iterator)->SeqHandle->logSize - ITR(iterator)->index - 1));
	*(ITR(iterator)->SeqHandle->head + ITR(iterator)->index) = newElement;
}

/* �������, ��������� ������ ������� ���������� */
void LSQ_DeleteFrontElement(LSQ_HandleT handle){
	TypeIterator *iterator = ITR(LSQ_GetElementByIndex(handle, 0));

	if (iterator == NULL)
		return;
	LSQ_DeleteGivenElement(iterator);
	LSQ_DestroyIterator(iterator);
}

/* �������, ��������� ��������� ������� ���������� */
void LSQ_DeleteRearElement(LSQ_HandleT handle){
	TypeIterator *iterator = NULL;
	if	(handle == NULL)
		return;
	iterator = ITR(LSQ_GetElementByIndex(handle,SEQ(handle)->logSize - 1));
	if (iterator == NULL)
		return;
	LSQ_DeleteGivenElement(iterator);
	LSQ_DestroyIterator(iterator);
}

/* �������, ��������� ������� ����������, ����������� �������� ����������. ��� ����������� �������� ��������� ��     *
 * ���� ������� � ������� ������.                                                                                    */
void LSQ_DeleteGivenElement(LSQ_IteratorT iterator){
	if (ITR(iterator) == NULL || !LSQ_IsIteratorDereferencable(iterator)) return;
	ITR(iterator)->SeqHandle->logSize--;
	memmove(ITR(iterator)->SeqHandle->head + ITR(iterator)->index , ITR(iterator)->SeqHandle->head + ITR(iterator)->index + 1, sizeof(LSQ_BaseTypeT)*(ITR(iterator)->SeqHandle->logSize - ITR(iterator)->index));
	ResizeSequense(ITR(iterator)->SeqHandle,-1);
}