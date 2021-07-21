# k-medoids-clustering

Singly Linked List를 이용한 K-Medoids Clustering 알고리즘 구현

- K-Medoids Clustering: K-Medoids Clustering은 주어진 데이터셋을 K개의 군집으로 묶는 clustering 알고리즘이다. clustering은 기계학습 알고리즘 중 비지도학습(Unsupervised learning)의 일종으로, 비지도학습은 지도학습과 달리 클래스 레이블(Weighted KNN 숙제에서의 정보(고등학생/유치원))이 존재하지 않는 데이터들을 가지고 유의미한 정보를 도출하는 학습 방법이다.

각각의 cluster를 singly linked list 자료구조를 사용하여 구현한다. K값과 데이터셋이 주어졌을 때, 데이터들을 K개의 군집으로 묶어 결과를 output.txt 파일로 출력한다. K개의 singly linked list의 head pointer를 저장하고 있는 pointer array가 있고, 각 linked list는 첫 번째 node에 medoid 정보가, 그 이후의 노드들에 해당 군집에 속하는 데이터들의 정보가 저장되도록 구현해야 한다. 예를 들어, 어떤 군집의 medoid가 (1.5, 3.1)이고, 해당 군집에 속해있는 데이터들이 (1.4, 3.41), (1.12, 3.21), (1.6, 3.56) 이라고 할 때, 해당 군집의 linked list는 [(1.5, 
3.1)] -> [(1.4, 3.41)] -> [(1.12, 3.21)] -> [(1.6, 3.56)] 이어야 한다. 만약 알고리즘의 iteration 과정에서 특정 데이터의 군집이 바뀌었다면, 원래 군집 linked list에서 해당 데이터를 삭제한 뒤 새로운 군집의 linked list에 데이터를 추가하는 작업이 필요하다.
