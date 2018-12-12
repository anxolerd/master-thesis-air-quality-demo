package io.github.anxolerd.airquality_demo.domain

data class Record(
    val title: String,
    var value: Double?,
    val unit: String,
    val min: Double = Double.NEGATIVE_INFINITY,
    val max: Double = Double.POSITIVE_INFINITY
)