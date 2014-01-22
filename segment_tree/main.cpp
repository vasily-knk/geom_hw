#include "stdafx.h"
#include "primitives.h"
//#include "segment_windowing.h"
#include "segment_tree.h"
#include "visualization/viewer_adapter.h"
#include "visualization/draw_util.h"

namespace visualization
{

    struct segment_tree_viewer
        : viewer_adapter    
    {
        segment_tree_viewer()
            : query_mode_(false)
        {
 
        }

        void draw(drawer_type & drawer) const override
        {
            for (size_t i = 0; i < segments_.size(); ++i)
            {
                drawer.set_color(indices_.count(i) ? Qt::red : Qt::blue);
                //drawer.set_color(Qt::blue);
                draw_segment(drawer, segments_.at(i));
            }

            if (new_segment_)
            {
                drawer.set_color(Qt::cyan);
                draw_segment(drawer, *new_segment_);
            }

            if (query_)
            {
                drawer.set_color(Qt::darkGreen);
                draw_segment(drawer, *query_);
            }
        }

        bool on_press(point_type const & pt) override
        {
            if (!query_mode_)
            {
                if (!new_segment_)
                {
                    new_segment_ = segment_t(pt, pt);
                }
                else
                {
                    segments_.push_back(*new_segment_);
                    new_segment_.reset();
                }
            }
            else 
            {
                if (!query_)
                {
                    query_ = segment_t(pt, pt);
                }
                else
                {
                    query_.reset();
                }
            }
            return true;
        }

        bool on_key(int key)
        {
            if (key == Qt::Key_Space)
            {
                switch_mode();
                return true;
            }
            return false;
        }

        bool on_move(point_type const &pt) override
        {
            last_mouse_ = pt;
            if (new_segment_)
            {
                (*new_segment_)[1] = pt;
                return true;
            }
            else if (query_)
            {
                (*query_)[0] = point_t(pt.x, (*query_)[0].y);
                (*query_)[1] = pt;
                update_indices();
                return true;
            }
            return false;
        }
    private:
        void draw_segment(drawer_type & drawer, const segment_t &s) const
        {
            drawer.draw_point(s[0], 3);
            drawer.draw_point(s[1], 3);
            drawer.draw_line(s, 1);
        }

        void draw_rect(drawer_type & drawer, const segment_t &s) const 
        {
            const point_t p1 = s[0];
            const point_t p2 = point_t(s[0].x, s[1].y);
            const point_t p3 = s[1];
            const point_t p4 = point_t(s[1].x, s[0].y);

            drawer.draw_line(p1, p2);
            drawer.draw_line(p2, p3);
            drawer.draw_line(p3, p4);
            drawer.draw_line(p4, p1);
        }

        void switch_mode()
        {
            new_segment_.reset();
            query_.reset();
            query_mode_ = !query_mode_;

            if (query_mode_)
                sement_tree_ = boost::make_shared<segment_tree_t>(segments_);
        }

        void update_indices()
        {
            segment_t q = *query_;
            range_t y_range(q[0].y, q[1].y);
            if (y_range.inf > y_range.sup)
                std::swap(y_range.inf, y_range.sup);
            segment_tree_t::query_t query(q[0].x, y_range);

            indices_ = sement_tree_->query(query);
        }

    private:
        vector<segment_t> segments_; 
        optional<segment_t> new_segment_, query_;
        point_t last_mouse_;

        shared_ptr<segment_tree_t> sement_tree_;
        unordered_set<uint32_t> indices_;

        bool query_mode_;
    };
}



int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    visualization::segment_tree_viewer viewer;
    visualization::run_viewer(&viewer, "Segment tree");
	return 0;
}
