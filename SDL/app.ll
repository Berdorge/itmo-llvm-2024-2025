; ModuleID = 'app.c'
source_filename = "app.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noreturn nounwind uwtable
define dso_local void @app() local_unnamed_addr #0 {
  %1 = alloca [32 x [64 x i32]], align 16
  %2 = alloca [32 x [64 x i32]], align 16
  call void @llvm.lifetime.start.p0(i64 8192, ptr nonnull %1) #5
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(8192) %1, i8 0, i64 8192, i1 false)
  call void @llvm.lifetime.start.p0(i64 8192, ptr nonnull %2) #5
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(8192) %2, i8 0, i64 8192, i1 false)
  %3 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 15, i64 32
  store i32 1, ptr %3, align 16, !tbaa !5
  %4 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 15, i64 33
  store i32 1, ptr %4, align 4, !tbaa !5
  %5 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 16, i64 31
  store i32 1, ptr %5, align 4, !tbaa !5
  %6 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 16, i64 32
  store i32 1, ptr %6, align 16, !tbaa !5
  %7 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 17, i64 32
  store i32 1, ptr %7, align 16, !tbaa !5
  br label %8

8:                                                ; preds = %17, %0
  %9 = phi i64 [ 0, %0 ], [ %18, %17 ]
  %10 = icmp ne i64 %9, 0
  %11 = add nsw i64 %9, -1
  %12 = icmp eq i64 %9, 31
  %13 = icmp eq i64 %9, 31
  %14 = add nuw nsw i64 %9, 1
  br label %19

15:                                               ; preds = %102
  %16 = icmp eq i64 %14, 32
  br i1 %16, label %118, label %17

17:                                               ; preds = %15, %122
  %18 = phi i64 [ %14, %15 ], [ 0, %122 ]
  br label %8, !llvm.loop !9

19:                                               ; preds = %8, %102
  %20 = phi i64 [ 0, %8 ], [ %116, %102 ]
  %21 = icmp ne i64 %20, 0
  %22 = and i1 %10, %21
  br i1 %22, label %23, label %29

23:                                               ; preds = %19
  %24 = add nsw i64 %20, -1
  %25 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 %11, i64 %24
  %26 = load i32, ptr %25, align 4, !tbaa !5
  %27 = icmp ne i32 %26, 0
  %28 = zext i1 %27 to i32
  br label %38

29:                                               ; preds = %19
  br i1 %10, label %38, label %30

30:                                               ; preds = %29
  %31 = icmp eq i64 %20, 63
  br i1 %31, label %64, label %32

32:                                               ; preds = %30
  %33 = add nuw nsw i64 %20, 1
  %34 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 %9, i64 %33
  %35 = load i32, ptr %34, align 4, !tbaa !5
  %36 = icmp ne i32 %35, 0
  %37 = zext i1 %36 to i32
  br label %71

38:                                               ; preds = %29, %23
  %39 = phi i32 [ %28, %23 ], [ 0, %29 ]
  %40 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 %11, i64 %20
  %41 = load i32, ptr %40, align 4, !tbaa !5
  %42 = icmp ne i32 %41, 0
  %43 = zext i1 %42 to i32
  %44 = add nuw nsw i32 %39, %43
  %45 = icmp ne i64 %20, 63
  %46 = and i1 %10, %45
  br i1 %46, label %47, label %54

47:                                               ; preds = %38
  %48 = add nuw nsw i64 %20, 1
  %49 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 %11, i64 %48
  %50 = load i32, ptr %49, align 4, !tbaa !5
  %51 = icmp ne i32 %50, 0
  %52 = zext i1 %51 to i32
  %53 = add nuw nsw i32 %44, %52
  br label %56

54:                                               ; preds = %38
  br i1 %45, label %56, label %55

55:                                               ; preds = %54
  br i1 %12, label %93, label %64

56:                                               ; preds = %47, %54
  %57 = phi i32 [ %53, %47 ], [ %44, %54 ]
  %58 = add nuw nsw i64 %20, 1
  %59 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 %9, i64 %58
  %60 = load i32, ptr %59, align 4, !tbaa !5
  %61 = icmp ne i32 %60, 0
  %62 = zext i1 %61 to i32
  %63 = add nuw nsw i32 %57, %62
  br i1 %13, label %92, label %71

64:                                               ; preds = %30, %55
  %65 = phi i32 [ 0, %30 ], [ %44, %55 ]
  %66 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 %14, i64 %20
  %67 = load i32, ptr %66, align 4, !tbaa !5
  %68 = icmp ne i32 %67, 0
  %69 = zext i1 %68 to i32
  %70 = add nuw nsw i32 %65, %69
  br label %84

71:                                               ; preds = %56, %32
  %72 = phi i32 [ %37, %32 ], [ %63, %56 ]
  %73 = phi i64 [ %33, %32 ], [ %58, %56 ]
  %74 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 %14, i64 %73
  %75 = load i32, ptr %74, align 4, !tbaa !5
  %76 = icmp ne i32 %75, 0
  %77 = zext i1 %76 to i32
  %78 = add nuw nsw i32 %72, %77
  %79 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 %14, i64 %20
  %80 = load i32, ptr %79, align 4, !tbaa !5
  %81 = icmp ne i32 %80, 0
  %82 = zext i1 %81 to i32
  %83 = add nuw nsw i32 %78, %82
  br i1 %21, label %84, label %102

84:                                               ; preds = %64, %71
  %85 = phi i32 [ %70, %64 ], [ %83, %71 ]
  %86 = add nsw i64 %20, -1
  %87 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 %14, i64 %86
  %88 = load i32, ptr %87, align 4, !tbaa !5
  %89 = icmp ne i32 %88, 0
  %90 = zext i1 %89 to i32
  %91 = add nuw nsw i32 %85, %90
  br label %96

92:                                               ; preds = %56
  br i1 %21, label %93, label %102

93:                                               ; preds = %55, %92
  %94 = phi i32 [ %63, %92 ], [ %44, %55 ]
  %95 = add nsw i64 %20, -1
  br label %96

96:                                               ; preds = %93, %84
  %97 = phi i64 [ %95, %93 ], [ %86, %84 ]
  %98 = phi i32 [ %94, %93 ], [ %91, %84 ]
  %99 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 %9, i64 %97
  %100 = load i32, ptr %99, align 4, !tbaa !5
  %101 = icmp ne i32 %100, 0
  br label %102

102:                                              ; preds = %71, %96, %92
  %103 = phi i32 [ %63, %92 ], [ %98, %96 ], [ %83, %71 ]
  %104 = phi i1 [ false, %92 ], [ %101, %96 ], [ false, %71 ]
  %105 = zext i1 %104 to i32
  %106 = add nuw nsw i32 %103, %105
  %107 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 %9, i64 %20
  %108 = load i32, ptr %107, align 4, !tbaa !5
  %109 = icmp eq i32 %108, 0
  %110 = icmp eq i32 %106, 3
  %111 = and i32 %106, -2
  %112 = icmp eq i32 %111, 2
  %113 = select i1 %109, i1 %110, i1 %112
  %114 = zext i1 %113 to i32
  %115 = getelementptr inbounds [32 x [64 x i32]], ptr %2, i64 0, i64 %9, i64 %20
  store i32 %114, ptr %115, align 4
  %116 = add nuw nsw i64 %20, 1
  %117 = icmp eq i64 %116, 64
  br i1 %117, label %15, label %19, !llvm.loop !11

118:                                              ; preds = %15, %123
  %119 = phi i32 [ %124, %123 ], [ 0, %15 ]
  %120 = lshr i32 %119, 3
  %121 = zext nneg i32 %120 to i64
  br label %126

122:                                              ; preds = %123
  tail call void (...) @simFlush() #5
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 16 dereferenceable(8192) %1, ptr noundef nonnull align 16 dereferenceable(8192) %2, i64 8192, i1 false), !tbaa !5
  br label %17

123:                                              ; preds = %126
  %124 = add nuw nsw i32 %119, 1
  %125 = icmp eq i32 %124, 256
  br i1 %125, label %122, label %118, !llvm.loop !12

126:                                              ; preds = %118, %126
  %127 = phi i32 [ 0, %118 ], [ %134, %126 ]
  %128 = lshr i32 %127, 3
  %129 = zext nneg i32 %128 to i64
  %130 = getelementptr inbounds [32 x [64 x i32]], ptr %1, i64 0, i64 %121, i64 %129
  %131 = load i32, ptr %130, align 4, !tbaa !5
  %132 = icmp eq i32 %131, 0
  %133 = select i1 %132, i32 -16777216, i32 -1
  tail call void @simPutPixel(i32 noundef %127, i32 noundef %119, i32 noundef %133) #5
  %134 = add nuw nsw i32 %127, 1
  %135 = icmp eq i32 %134, 512
  br i1 %135, label %123, label %126, !llvm.loop !13
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #2

declare void @simPutPixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #3

declare void @simFlush(...) local_unnamed_addr #3

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #4

attributes #0 = { noreturn nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #3 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"Ubuntu clang version 18.1.3 (1ubuntu1)"}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = distinct !{!9, !10}
!10 = !{!"llvm.loop.mustprogress"}
!11 = distinct !{!11, !10}
!12 = distinct !{!12, !10}
!13 = distinct !{!13, !10}
