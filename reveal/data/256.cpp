IN_PROC_BROWSER_TEST_F ( HttpsEngagementPageLoadMetricsBrowserTest , BackgroundThenForeground_Http ) {
 StartHttpServer ( ) ;
 base : : TimeDelta upper_bound = NavigateInBackgroundAndCloseInForegroundWithTiming ( http_test_server_ -> GetURL ( "/simple.html" ) ) ;
 histogram_tester_ . ExpectTotalCount ( internal : : kHttpEngagementHistogram , 1 ) ;
 histogram_tester_ . ExpectTotalCount ( internal : : kHttpsEngagementHistogram , 0 ) ;
 int32_t bucket_min = histogram_tester_ . GetAllSamples ( internal : : kHttpEngagementHistogram ) [ 0 ] . min ;
 EXPECT_GE ( upper_bound . InMilliseconds ( ) , bucket_min ) ;
 EXPECT_LT ( 0 , bucket_min ) ;
 }