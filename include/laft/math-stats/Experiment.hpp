#ifndef LAFT_MATH_STATS_EXPERIMENT_HPP_INCLUDED
#define LAFT_MATH_STATS_EXPERIMENT_HPP_INCLUDED

namespace laft
{
	namespace math
	{
		namespace stats
		{
			namespace impl
			{
				// Utilities
				template <typename Prob, typename EventType, EventType event>
				struct ExperimentEvent
				{
					ExperimentEvent() :
						prob_{0}
					{}
					Prob prob_;		
				};
				
				template <typename Prob, typename EventType, EventType ...events>
				class ExperimentImpl;
				template <typename Prob, typename EventType, EventType event, EventType ... rest>
				class ExperimentImpl<Prob, EventType, event, rest...> :
					public ExperimentEvent<Prob, EventType, event>,
					public ExperimentImpl<Prob, EventType, rest...>
				{
				};
				template <typename Prob, typename EventType>
				class ExperimentImpl<Prob, EventType>
				{
				};
			}
			
			/**
				\brief A random experiment that can produce one result or none.
				\tparam Prob Type used to define the probability of each event.
				\tparam EventType Type used to represent the differents events.
				\tparam events List of event that can happen.
			*/
			template <typename Prob, typename EventType, EventType ...events>
			class Experiment :
				private impl::ExperimentImpl<Prob, EventType, events...>
			{
				public:
					template <EventType event>
					Prob const& get() const& { return impl::ExperimentEvent<Prob, EventType, event>::prob_; }
					template <EventType event>
					void set(Prob const& prob) { impl::ExperimentEvent<Prob, EventType, event>::prob_ = prob; }
			};
		}
	}
}

#endif
