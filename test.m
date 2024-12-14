clc
A = randi(28,4,7) % reshape(randperm(28),4,7)
A = [sort(A(1,:));
     sort(A(2,:),'descend');
     sort(A(3,:));
     sort(A(4,:),'descend')]
A = [min(A(1:2,:));
     max(A(1:2,:));
     max(A(3:4,:));
     min(A(3:4,:))]
A = [sort(A(1,:));
     sort(A(2,:));
     sort(A(3,:),'descend');
     sort(A(4,:),'descend')]
A = [min(A([1,3],:));
     min(A([2,4],:));
     max(A([1,3],:));
     max(A([2,4],:))]
A = [min(A(1:2,:));
     max(A(1:2,:));
     min(A(3:4,:));
     max(A(3:4,:))]
A = [sort(A(1,:));
     sort(A(2,:));
     sort(A(3,:));
     sort(A(4,:))]