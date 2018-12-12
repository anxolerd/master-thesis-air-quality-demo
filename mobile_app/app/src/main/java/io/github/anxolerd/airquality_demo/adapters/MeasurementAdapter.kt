package io.github.anxolerd.airquality_demo.adapters

import android.support.v7.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import io.github.anxolerd.airquality_demo.R
import io.github.anxolerd.airquality_demo.domain.Record


class MeasurementAdapter(private var measurements: List<Record>) :
    RecyclerView.Adapter<MeasurementAdapter.ViewHolder>() {
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val ctx = parent.context
        val inflater = LayoutInflater.from(ctx)

        val recordView = inflater.inflate(R.layout.item_measurement, parent, false)

        return ViewHolder(recordView)
    }

    override fun onBindViewHolder(viewHolder: ViewHolder, position: Int) {
        val record = measurements[position]

        viewHolder.nameText.text = "${record.title}:"
        viewHolder.valueText.text = when {
            record.value == null -> "No data"
            else -> "${record.value}${record.unit}"
        }

        when {
            record.value == null -> viewHolder.status.setImageResource(android.R.drawable.ic_delete)
            record.value!! < record.min -> viewHolder.status.setImageResource(android.R.drawable.arrow_down_float)
            record.value!! < record.max -> viewHolder.status.setImageResource(android.R.drawable.checkbox_on_background)
            else -> viewHolder.status.setImageResource(android.R.drawable.arrow_up_float)
        }
    }


    override fun getItemCount(): Int {
        return measurements.size
    }

    inner class ViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        var nameText: TextView
        var valueText: TextView
        var status: ImageView

        init {
            nameText = itemView.findViewById(R.id.measurement_name)
            valueText = itemView.findViewById(R.id.measurement_value)
            status = itemView.findViewById(R.id.measurement_status_icon)
        }
    }
}